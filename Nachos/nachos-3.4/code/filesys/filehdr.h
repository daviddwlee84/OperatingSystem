// filehdr.h 
//	Data structures for managing a disk file header.  
//
//	A file header describes where on disk to find the data in a file,
//	along with other information about the file (for instance, its
//	length, owner, etc.)
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"

#ifndef FILEHDR_H
#define FILEHDR_H

#include "disk.h"
#include "bitmap.h"
#include <time.h> // Lab5: for created time, modified time, last visited time
#include <libgen.h> // Lab5: for dirname and basename

// Disk part
#define NumOfIntHeaderInfo 2
#define NumOfTimeHeaderInfo 3
#define LengthOfTimeHeaderStr 26 // 25 + 1 ('/0')
#define MaxExtLength 5           // 4  + 1 ('/0')
#define LengthOfAllString MaxExtLength + NumOfTimeHeaderInfo*LengthOfTimeHeaderStr
// In-core part
// #define MaxPathLength 30 // uncomment when we need it

// FileHeader Object Data (for more detail see how this object been store in FileHeader::WriteBack)
// Header Data | dataSectors
// NumDirect is the sector starting point of the "data", that is we need to keep the space for the header informations
#ifndef INDIRECT_MAP
#define NumDirect 	((SectorSize - (NumOfIntHeaderInfo*sizeof(int) + LengthOfAllString*sizeof(char))) / sizeof(int))
#define MaxFileSize 	(NumDirect * SectorSize)
#else
#define NumDataSectors ((SectorSize - (NumOfIntHeaderInfo*sizeof(int) + LengthOfAllString*sizeof(char))) / sizeof(int))
#define NumDirect (NumDataSectors - 2)
#define IndirectSectorIdx (NumDataSectors - 2)
#define DoubleIndirectSectorIdx (NumDataSectors - 1)
#define MaxFileSize (NumDirect * SectorSize) + \
                    ((SectorSize / sizeof(int)) * SectorSize) + \
                    ((SectorSize / sizeof(int)) * ((SectorSize / sizeof(int)) * SectorSize))
#endif

// The following class defines the Nachos "file header" (in UNIX terms,  
// the "i-node"), describing where on disk to find all of the data in the file.
// The file header is organized as a simple table of pointers to
// data blocks. 
//
// The file header data structure can be stored in memory or on disk.
// When it is on disk, it is stored in a single sector -- this means
// that we assume the size of this data structure to be the same
// as one disk sector.  Without indirect addressing, this
// limits the maximum file length to just under 4K bytes.
//
// There is no constructor; rather the file header can be initialized
// by allocating blocks for the file (if it is a new file), or by
// reading it from disk.

class FileHeader {
  public:
    bool Allocate(BitMap *bitMap, int fileSize);// Initialize a file header, 
						//  including allocating space 
						//  on disk for the file data
    void Deallocate(BitMap *bitMap);  		// De-allocate this file's 
						//  data blocks

    void FetchFrom(int sectorNumber); 	// Initialize file header from disk
    void WriteBack(int sectorNumber); 	// Write modifications to file header
					//  back to disk

    int ByteToSector(int offset);	// Convert a byte offset into the file
					// to the disk sector containing
					// the byte

    int FileLength();			// Return the length of the file 
					// in bytes

    void Print();			// Print the contents of the file.

    // Lab5: additional file attributes
    void HeaderCreateInit(char* ext); // Initialize all header message for creation
    // Disk part
    void setFileType(char* ext) { strcmp(ext, "") ? strcpy(fileType, ext) : strcpy(fileType, "None"); }
    void setCreateTime(char* t) { strcpy(createdTime, t); }
    void setModifyTime(char* t) { strcpy(modifiedTime, t); }
    void setVisitTime(char* t) { strcpy(lastVisitedTime, t); }
    char* getFileType() { return strdup(fileType); }
    // In-core part
    void setHeaderSector(int sector) { headerSector = sector; }
    int getHeaderSector() { return headerSector; }
    // void setFilePath(char* path) { strcpy(filePath, path); } // uncomment when we need it

    // Lab5: expand file size
    bool ExpandFileSize(BitMap *freeMap, int additionalBytes);

  private:
	/*
		Hint from the web:
		You will need a data structure to store more information in a header.
		Fields in a class can be separated into disk part and in-core part.
		Disk part are data that will be written into disk.
		In-core part are data only lies in memory, and are used to maintain the data structure of this class.
		In order to implement a data structure, you will need to add some "in-core" data
		to maintain data structure.
		
		Disk Part - numBytes, numSectors, dataSectors occupy exactly 128 bytes and will be
		written to a sector on disk.
		In-core part - none
		
	*/
    // ======================== Disk Part ======================== //
    // == Header Information == //
    int numBytes;   // Number of bytes in the file
    int numSectors; // Number of data sectors in the file

    // Lab5: additional file attributes
    char fileType[MaxExtLength];
    char createdTime[LengthOfTimeHeaderStr];
    char modifiedTime[LengthOfTimeHeaderStr];
    char lastVisitedTime[LengthOfTimeHeaderStr];

    // == Data Sectors == //
#ifndef INDIRECT_MAP
    int dataSectors[NumDirect]; // Disk sector numbers for each data
                                // block in the file
#else
    int dataSectors[NumDataSectors]; // Disk sector numbers for each data
                                     // block in the file
                                     // the last two of them are indirect block
#endif
    // ======================== In-core Part ======================== //
    // This will be assign value when the file is open!
    int headerSector; // Because when we OpenFile, we need to update the header information
                      // but the sector message is only exist when create the OpenFile object
                      // some how we need to know which sector to write back
    // char filePath[MaxPathLength]; // uncomment when we need it
};

// Lab5: Multi-level directory
#define MAX_DIR_DEPTH 5 // Because we don't have malloc() so assign a max depth to initialize the array size

typedef struct {
    char* dirArray[MAX_DIR_DEPTH];
    int dirDepth; // if root dir, dir depth = 0
    char* base;
} FilePath;

char* printChar(char oriChar);
extern char* getFileExtension(char *filename);
extern char* getCurrentTime(void);
extern FilePath pathParser(char* path);

#endif // FILEHDR_H
