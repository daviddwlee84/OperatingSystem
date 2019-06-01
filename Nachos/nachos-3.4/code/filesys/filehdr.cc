// filehdr.cc 
//	Routines for managing the disk file header (in UNIX, this
//	would be called the i-node).
//
//	The file header is used to locate where on disk the 
//	file's data is stored.  We implement this as a fixed size
//	table of pointers -- each entry in the table points to the 
//	disk sector containing that portion of the file data
//	(in other words, there are no indirect or doubly indirect 
//	blocks). The table size is chosen so that the file header
//	will be just big enough to fit in one disk sector, 
//
//      Unlike in a real system, we do not keep track of file permissions, 
//	ownership, last modification date, etc., in the file header. 
//
//	A file header can be initialized in two ways:
//	   for a new file, by modifying the in-memory data structure
//	     to point to the newly allocated data blocks
//	   for a file already on disk, by reading the file header from disk
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#include "system.h"
#include "filehdr.h"

#define LevelMapNum (SectorSize / sizeof(int)) // 32 when SectorSize is 128 bytes

//----------------------------------------------------------------------
// FileHeader::Allocate
// 	Initialize a fresh file header for a newly created file.
//	Allocate data blocks for the file out of the map of free disk blocks.
//	Return FALSE if there are not enough free blocks to accomodate
//	the new file.
//
//	"freeMap" is the bit map of free disk sectors
//	"fileSize" is the bit map of free disk sectors
//----------------------------------------------------------------------

bool
FileHeader::Allocate(BitMap *freeMap, int fileSize)
{
    numBytes = fileSize;
    numSectors = divRoundUp(fileSize, SectorSize);
    if (freeMap->NumClear() < numSectors)
        return FALSE; // not enough space

    if (numSectors < NumDirect) {
        DEBUG('f', COLORED(OKGREEN, "Allocating using direct indexing only\n"));
        for (int i = 0; i < numSectors; i++)
            dataSectors[i] = freeMap->Find();
    } else {
#ifndef INDIRECT_MAP
        ASSERT_MSG(FALSE, "File size exceeded the maximum representation of the direct map");
#else
        if (numSectors < (NumDirect + LevelMapNum)) {
            DEBUG('f', COLORED(OKGREEN, "Allocating using single indirect indexing\n"));
            // direct
            for (int i = 0; i < NumDirect; i++)
                dataSectors[i] = freeMap->Find();
            // indirect
            dataSectors[IndirectSectorIdx] = freeMap->Find();
            int indirectIndex[LevelMapNum];
            for (int i = 0; i < numSectors - NumDirect; i++) {
                indirectIndex[i] = freeMap->Find();
            }
            synchDisk->WriteSector(dataSectors[IndirectSectorIdx], (char*)indirectIndex);
        } else if (numSectors < (NumDirect + LevelMapNum + LevelMapNum*LevelMapNum)) {
            DEBUG('f', COLORED(OKGREEN, "Allocating using double indirect indexing\n"));
            // direct
            for (int i = 0; i < NumDirect; i++)
                dataSectors[i] = freeMap->Find();
            dataSectors[IndirectSectorIdx] = freeMap->Find();
            // first indirect
            int indirectIndex[LevelMapNum];
            for (int i = 0; i < LevelMapNum; i++) {
                indirectIndex[i] = freeMap->Find();
            }
            synchDisk->WriteSector(dataSectors[IndirectSectorIdx], (char*)indirectIndex);
            // second indirect
            dataSectors[DoubleIndirectSectorIdx] = freeMap->Find();
            const int sectorsLeft = numSectors - NumDirect - LevelMapNum;
            const int secondIndirectNum = divRoundUp(sectorsLeft, LevelMapNum);
            int doubleIndirectIndex[LevelMapNum];
            for (int j = 0; j < secondIndirectNum; j++) {
                doubleIndirectIndex[j] = freeMap->Find();
                int singleIndirectIndex[LevelMapNum];
                for (int i = 0; (i < LevelMapNum) && (i + j * LevelMapNum < sectorsLeft); i++) {
                    singleIndirectIndex[i] = freeMap->Find();
                }
                synchDisk->WriteSector(doubleIndirectIndex[j], (char*)singleIndirectIndex);
            }
            synchDisk->WriteSector(dataSectors[DoubleIndirectSectorIdx], (char*)doubleIndirectIndex);
        } else {
            ASSERT_MSG(FALSE, "File size exceeded the maximum representation of the double indirect mapping");
        }
#endif
    }
    return TRUE;
}

//----------------------------------------------------------------------
// FileHeader::Deallocate
// 	De-allocate all the space allocated for data blocks for this file.
//
//	"freeMap" is the bit map of free disk sectors
//----------------------------------------------------------------------

void
FileHeader::Deallocate(BitMap *freeMap)
{
#ifndef INDIRECT_MAP
    for (int i = 0; i < numSectors; i++) {
        ASSERT(freeMap->Test((int)dataSectors[i])); // ought to be marked!
        freeMap->Clear((int)dataSectors[i]);
    }
#else
    int i, ii, iii; // For direct / single indirect / double indirect indexing
    DEBUG('f', COLORED(OKGREEN, "Deallocating direct indexing table\n"));
    for (i = 0; (i < numSectors) && (i < NumDirect); i++) {
        ASSERT(freeMap->Test((int)dataSectors[i])); // ought to be marked!
        freeMap->Clear((int)dataSectors[i]);
    }
    if (numSectors > NumDirect) {
        DEBUG('f', COLORED(OKGREEN, "Deallocating single indirect indexing table\n"));
        int singleIndirectIndex[LevelMapNum]; // used to restore the indexing map
        synchDisk->ReadSector(dataSectors[IndirectSectorIdx], (char*)singleIndirectIndex);
        for (i = NumDirect, ii = 0; (i < numSectors) && (ii < LevelMapNum); i++, ii++) {
            ASSERT(freeMap->Test((int)singleIndirectIndex[ii])); // ought to be marked!
            freeMap->Clear((int)singleIndirectIndex[ii]);
        }
        // Free the sector of the single indirect indexing table
        ASSERT(freeMap->Test((int)dataSectors[IndirectSectorIdx]));
        freeMap->Clear((int)dataSectors[IndirectSectorIdx]);
        if (numSectors > NumDirect + LevelMapNum) {
            DEBUG('f', COLORED(OKGREEN, "Deallocating double indirect indexing table\n"));
            int doubleIndirectIndex[LevelMapNum];
            synchDisk->ReadSector(dataSectors[DoubleIndirectSectorIdx], (char*)doubleIndirectIndex);
            for (i = NumDirect + LevelMapNum, ii = 0; (i < numSectors) && (ii < LevelMapNum); ii++) {
                synchDisk->ReadSector(doubleIndirectIndex[ii], (char*)singleIndirectIndex);
                for (iii = 0; (i < numSectors) && (iii < LevelMapNum); i++, iii++) {
                    ASSERT(freeMap->Test((int)singleIndirectIndex[iii])); // ought to be marked!
                    freeMap->Clear((int)singleIndirectIndex[iii]);
                }
                // Free the sector of the single indirect indexing table
                ASSERT(freeMap->Test((int)doubleIndirectIndex[ii]));
                freeMap->Clear((int)doubleIndirectIndex[ii]);
            }
            // Free the sector of the single indirect indexing table
            ASSERT(freeMap->Test((int)dataSectors[DoubleIndirectSectorIdx]));
            freeMap->Clear((int)dataSectors[DoubleIndirectSectorIdx]);
        }
    }
#endif
}

//----------------------------------------------------------------------
// FileHeader::FetchFrom
// 	Fetch contents of file header from disk. 
//
//	"sector" is the disk sector containing the file header
//----------------------------------------------------------------------

void
FileHeader::FetchFrom(int sector)
{
    synchDisk->ReadSector(sector, (char *)this);
}

//----------------------------------------------------------------------
// FileHeader::WriteBack
// 	Write the modified contents of the file header back to disk. 
//
//	"sector" is the disk sector to contain the file header
//----------------------------------------------------------------------

void
FileHeader::WriteBack(int sector)
{
    synchDisk->WriteSector(sector, (char *)this); 
}

//----------------------------------------------------------------------
// FileHeader::ByteToSector
// 	Return which disk sector is storing a particular byte within the file.
//      This is essentially a translation from a virtual address (the
//	offset in the file) to a physical address (the sector where the
//	data at the offset is stored).
//
//	"offset" is the location within the file of the byte in question
//----------------------------------------------------------------------

int
FileHeader::ByteToSector(int offset)
{
#ifndef INDIRECT_MAP
    return (dataSectors[offset / SectorSize]);
#else
    const int directMapSize = NumDirect * SectorSize;
    const int singleIndirectMapSize = directMapSize + LevelMapNum * SectorSize;
    const int doubleIndirectMapSize = singleIndirectMapSize +  LevelMapNum * LevelMapNum * SectorSize;

    if (offset < directMapSize) {
        return (dataSectors[offset / SectorSize]);
    } else if (offset < singleIndirectMapSize) {
        const int sectorNum = (offset - directMapSize) / SectorSize;
        int singleIndirectIndex[LevelMapNum]; // used to restore the indexing map
        synchDisk->ReadSector(dataSectors[IndirectSectorIdx], (char*)singleIndirectIndex);
        return singleIndirectIndex[sectorNum];
    } else {
        const int indexSectorNum = (offset - singleIndirectMapSize) / SectorSize / LevelMapNum;
        const int sectorNum = (offset - singleIndirectMapSize) / SectorSize % LevelMapNum;
        int doubleIndirectIndex[LevelMapNum]; // used to restore the indexing map
        synchDisk->ReadSector(dataSectors[DoubleIndirectSectorIdx], (char*)doubleIndirectIndex);
        int singleIndirectIndex[LevelMapNum]; // used to restore the indexing map
        synchDisk->ReadSector(doubleIndirectIndex[indexSectorNum], (char*)singleIndirectIndex);
        return singleIndirectIndex[sectorNum];
    }
#endif
}

//----------------------------------------------------------------------
// FileHeader::FileLength
// 	Return the number of bytes in the file.
//----------------------------------------------------------------------

int
FileHeader::FileLength()
{
    return numBytes;
}

//----------------------------------------------------------------------
// FileHeader::Print
// 	Print the contents of the file header, and the contents of all
//	the data blocks pointed to by the file header.
//----------------------------------------------------------------------

void
FileHeader::Print()
{
    int i, j, k; // current sector / byte position in a sector / current byte position in file
    char *data = new char[SectorSize];

    // Lab5: additional file attributes
    printf("------------ %s -------------\n", COLORED(GREEN, "FileHeader contents"));
    printf("File type: %s\n", fileType);
    printf("Created: %s", createdTime);
    printf("Modified: %s", modifiedTime);
    printf("Last visited: %s", lastVisitedTime);
    // printf("\tPath: %s\n", filePath); // uncomment when we need it
    printf("File size: %d.  File blocks:\n", numBytes);
#ifndef INDIRECT_MAP
    for (i = 0; i < numSectors; i++)
        printf("%d ", dataSectors[i]);
    printf("\nFile contents:\n");
    for (i = k = 0; i < numSectors; i++)
    {
        synchDisk->ReadSector(dataSectors[i], data);
        for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++)
            printChar(data[j]);
        printf("\n"); // Reach the end of sector or the end of file
    }
#else
    int ii, iii; // For single / double indirect indexing
    int singleIndirectIndex[LevelMapNum]; // used to restore the indexing map
    int doubleIndirectIndex[LevelMapNum]; // used to restore the indexing map
    printf("  Direct indexing:\n    ");
    for (i = 0; (i < numSectors) && (i < NumDirect); i++)
        printf("%d ", dataSectors[i]);
    if (numSectors > NumDirect) {
        printf("\n  Indirect indexing: (mapping table sector: %d)\n    ", dataSectors[IndirectSectorIdx]);
        synchDisk->ReadSector(dataSectors[IndirectSectorIdx], (char*)singleIndirectIndex);
        for (i = NumDirect, ii = 0; (i < numSectors) && (ii < LevelMapNum); i++, ii++)
            printf("%d ", singleIndirectIndex[ii]);
        if (numSectors > NumDirect + LevelMapNum) {
            printf("\n  Double indirect indexing: (mapping table sector: %d)", dataSectors[DoubleIndirectSectorIdx]);
            synchDisk->ReadSector(dataSectors[DoubleIndirectSectorIdx], (char*)doubleIndirectIndex);
            for (i = NumDirect + LevelMapNum, ii = 0; (i < numSectors) && (ii < LevelMapNum); ii++) {
                printf("\n    single indirect indexing: (mapping table sector: %d)\n      ", doubleIndirectIndex[ii]);
                synchDisk->ReadSector(doubleIndirectIndex[ii], (char*)singleIndirectIndex);
                for (iii = 0;  (i < numSectors) && (iii < LevelMapNum); i++, iii++)
                    printf("%d ", singleIndirectIndex[iii]);
            }
        }
    }
    printf("\nFile contents:\n");
    for (i = k = 0; (i < numSectors) && (i < NumDirect); i++)
    {
        synchDisk->ReadSector(dataSectors[i], data);
        for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++)
            printChar(data[j]);
        printf("\n");
    }
    if (numSectors > NumDirect) {
        synchDisk->ReadSector(dataSectors[IndirectSectorIdx], (char*)singleIndirectIndex);
        for (i = NumDirect, ii = 0; (i < numSectors) && (ii < LevelMapNum); i++, ii++) {
            synchDisk->ReadSector(singleIndirectIndex[ii], data);
            for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++)
                printChar(data[j]);
            printf("\n");
        }
        if (numSectors > NumDirect + LevelMapNum) {
            synchDisk->ReadSector(dataSectors[DoubleIndirectSectorIdx], (char*)doubleIndirectIndex);
            for (i = NumDirect + LevelMapNum, ii = 0; (i < numSectors) && (ii < LevelMapNum); ii++) {
                synchDisk->ReadSector(doubleIndirectIndex[ii], (char*)singleIndirectIndex);
                for (iii = 0; (i < numSectors) && (iii < LevelMapNum); i++, iii++) {
                    synchDisk->ReadSector(singleIndirectIndex[iii], data);
                    for (j = 0; (j < SectorSize) && (k < numBytes); j++, k++)
                        printChar(data[j]);
                    printf("\n");
                }
            }
        }
    }
#endif
    printf("----------------------------------------------\n");
    delete[] data;
}

//----------------------------------------------------------------------
// FileHeader::HeaderCreateInit
// 	Set the file type, time informations and other attribute.
//  Invoke this when create a FileHeader first time.
//  (not every "new FileHeader")
//----------------------------------------------------------------------

void
FileHeader::HeaderCreateInit(char* ext)
{
    setFileType(ext);

    char* currentTimeString = getCurrentTime();
    setCreateTime(currentTimeString);
    setModifyTime(currentTimeString);
    setVisitTime(currentTimeString);
}

// Lab5: dynamic allocate file size

//----------------------------------------------------------------------
// FileHeader::ExpandFileSize
// 	Reallocate the file size for additionalBytes
//----------------------------------------------------------------------

bool
FileHeader::ExpandFileSize(BitMap *freeMap, int additionalBytes)
{
    ASSERT(additionalBytes > 0);
    numBytes += additionalBytes;
    int initSector = numSectors;
    numSectors = divRoundUp(numBytes, SectorSize);
    if (initSector == numSectors) {
        return TRUE; // no need more sector
    }
    int sectorsToExpand = numSectors - initSector;
    if (freeMap->NumClear() < sectorsToExpand) {
        return FALSE; // no more space to allocate
    }

    DEBUG('f', COLORED(OKGREEN, "Expanding file size for %d sectors (%d bytes)\n"), sectorsToExpand, additionalBytes);

    if (numSectors < NumDirect) { // just like FileHeader::Allocate
        for (int i = initSector; i < numSectors; i++)
            dataSectors[i] = freeMap->Find();
    } else {
#ifndef INDIRECT_MAP
        ASSERT_MSG(FALSE, "File size exceeded the maximum representation of the direct map");
#else

// TODO: Expand file size in indirect mapping mode

        if (numSectors < (NumDirect + LevelMapNum)) {
        } else if (numSectors < (NumDirect + LevelMapNum + LevelMapNum*LevelMapNum)) {
        } else {
            ASSERT_MSG(FALSE, "File size exceeded the maximum representation of the double indirect mapping");
        }
#endif
    }

    return TRUE;
}

// Lab5: Helper Functions

//----------------------------------------------------------------------
// printChar
//    Print character as char if it is readable, otherwise its value.
//
//    (move the logic from which originally in FileHeader::Print to here)
//----------------------------------------------------------------------

char*
printChar(char oriChar)
{
    if ('\040' <= oriChar && oriChar <= '\176') // isprint(oriChar)
        printf("%c", oriChar); // Character content
    else
        printf("\\%x", (unsigned char)oriChar); // Unreadable binary content
}

//----------------------------------------------------------------------
// getFileExtension
//    Extract the file name to get the extension. If the file name don't
//    have extension then return empty string. 
//
//      e.g. test.haha.pdf => "pdf"
//      e.g. test.txt      => "txt"
//      e.g. test.         => ""
//      e.g. test          => ""
//----------------------------------------------------------------------

char*
getFileExtension(char *filename)
{
    char *dot = strrchr(filename, '.');
    if(!dot || dot == filename) return "";
    return dot + 1;
}

//----------------------------------------------------------------------
// getCurrentTime
//    Return the sting of the time that we called it.
//
//    (use asctime to transfer to string)
//----------------------------------------------------------------------

char*
getCurrentTime(void)
{
    time_t rawtime;
    time(&rawtime);
    struct tm* currentTime = localtime(&rawtime);
    return asctime(currentTime); // This somehow will generate extra '\n'
}

//----------------------------------------------------------------------
// pathParser
//    Extract the filePath into dirname and base name.
//    The retuen value is using the call by reference.
// 
//    filePath: "/foo/bar/baz.txt"
//    dir: ["foo", "bar"]
//    base: "baz.txt"
//----------------------------------------------------------------------

FilePath
pathParser(char* path)
{
    if (path[0] == '/')
        path = &path[1]; // Don't count the first '/'

    char* ts1 = strdup(path);
    char* ts2 = strdup(path);

    FilePath filepath;

    // The return of basename() will be the copy pointer of input!!
    char* currentDir = dirname(ts1);
    filepath.base = strdup(basename(ts2)); 

    // See how depth the path is
    int depth;
    for (depth = 0; path[depth]; path[depth] == '/' ? depth++ : *path++);
    filepath.dirDepth = depth;
    ASSERT_MSG(depth <= MAX_DIR_DEPTH, "The file path depth is exceed the max directory depth");

    // Not in current directory. Travel to the directory
    while (strcmp(currentDir, ".")) { // while currentDir is not "."
        filepath.dirArray[--depth] = strdup(basename(currentDir));
        currentDir = dirname(currentDir);
    }

    return filepath;
}
