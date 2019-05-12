# Lab 5: File System

> Recommendation of this Lab
>
> 1. Modify and management of the data structure
>     * Adding the file attribute => modify file header
>     * Hierarchical directory should be able to create directory or file
>       * Initialize the file header message by its file type
> 2. When implementing hierarchical (multi-level) directory:
>     * The meaning of the directory file
>       * Each directory match a file. Use the file can get the parent and children directory information
>     * The directory file size is pre-defined, but in practice the size should based on the content and can be changed
>     * After implement the hierarchical directory, add or delete the directory entry must according specific path (it's important to understand the "tree traversal")
> 3. To implement the infinity file length, it's good to try to use mixed indexed allocation method

Test the original file system:

* Before format the disk (or delete the `code/filesys/DISK` file)

    ```txt
    $ docker run -it nachos_filesys nachos/nachos-3.4/code/filesys/nachos -D
    Bit map file header:
    FileHeader contents.  File size: 0.  File blocks:

    File contents:
    Directory file header:
    FileHeader contents.  File size: 0.  File blocks:

    File contents:
    Bitmap set:

    Directory contents:
    ```

* Format the disk and show the debug message

    ```txt
    $ docker run -it nachos_filesys nachos/nachos-3.4/code/filesys/nachos -f -d d
    Initializing the disk, 0x8054a56 0x818f258
    Request latency = 16480
    Writing to sector 0
    Writing sector: 0
    80 1 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    Updating last sector = 0, 0
    Request latency = 16470
    Writing to sector 1
    Writing sector: 1
    c8 2 3 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    Updating last sector = 1, 0
    Request latency = 500
    Reading from sector 0
    Reading sector: 0
    80 1 2 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    Updating last sector = 0, 0
    Request latency = 500
    Reading from sector 1
    Reading sector: 1
    c8 2 3 4 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    Updating last sector = 1, 0
    Request latency = 15410
    Writing to sector 2
    Writing sector: 2
    1f 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    Updating last sector = 2, 0
    Request latency = 500
    Reading from sector 4
    Reading sector: 4
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    Updating last sector = 4, 0
    Request latency = 15940
    Writing to sector 3
    Writing sector: 3
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    Updating last sector = 3, 0
    Request latency = 16470
    Writing to sector 4
    Writing sector: 4
    0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
    Updating last sector = 4, 0
    ```

* Format and show the directory

    ```txt
    $ docker run -it nachos_filesys nachos/nachos-3.4/code/filesys/nachos -f -D  
    Bit map file header:
    FileHeader contents.  File size: 128.  File blocks:
    2
    File contents:
    \1f\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0
    Directory file header:
    FileHeader contents.  File size: 200.  File blocks:
    3 4
    File contents:
    \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0
    \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0
    Bitmap set:
    0, 1, 2, 3, 4,
    Directory contents:
    ```

* Copy file to disk and print

    ```txt
    $ docker run -it nachos_filesys nachos/nachos-3.4/code/filesys/nachos -f -cp nachos/nachos-3.4/code/filesys/test/big big -p big
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    This is the spring of our discontent.
    ```

* Run my test script that I put in `code/filesys/test/my_test_script1.sh` using `docker run -it nachos_filesys nachos/nachos-3.4/code/filesys/test/my_test_script1.sh` (I've set it to executable file). Here is the content of the script.
  
    > I've add additional `-Q` flag functionality in `code/threads/main.cc` that will disable the verbose message of machine. It's useful when we execute multiple file system commands and it won't be mess.

    ```sh
    #!/bin/sh
    # goto filesys/ in docker
    cd /nachos/nachos-3.4/code/filesys

    # use -Q to disable verbose machine messages
    echo "=== format the DISK ==="
    ./nachos -Q -f
    echo "=== copies file \"big\" from UNIX to Nachos ==="
    ./nachos -Q -cp test/big big
    # Files: big
    echo "=== copies file \"small\" from UNIX to Nachos ==="
    ./nachos -Q -cp test/small small
    # Files: big, small
    echo "=== lists the contents of the Nachos directory ==="
    ./nachos -Q -l
    # big
    # small
    echo "=== remove the file \"big\" from Nachos ==="
    ./nachos -Q -r big
    echo "=== print the content of file \"small\" ==="
    ./nachos -Q -p small
    echo "=== prints the contents of the entire file system ==="
    ./nachos -Q -D
    echo "=== tests the performance of the Nachos file system ==="
    ./nachos -Q -t
    ```

    ```txt
    $ docker run -it nachos_filesys nachos/nachos-3.4/code/filesys/test/my_test_script1.sh
    === format the DISK ===
    === copies file "big" from UNIX to Nachos ===
    === copies file "small" from UNIX to Nachos ===
    === lists the contents of the Nachos directory ===
    big
    small
    === remove the file "big" from Nachos ===
    === print the content of file "small" ===
    This is the spring of our discontent.
    === prints the contents of the entire file system ===
    Bit map file header:
    FileHeader contents.  File size: 128.  File blocks:
    2
    File contents:
    \1f\18\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0
    Directory file header:
    FileHeader contents.  File size: 200.  File blocks:
    3 4
    File contents:
    \0\0\0\0\5\0\0\0big\0\0\0\0\0\0\0\0\0\1\0\0\0\b\0\0\0small\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0
    \0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0
    Bitmap set:
    0, 1, 2, 3, 4, 11, 12,
    Directory contents:
    Name: small, Sector: 11
    FileHeader contents.  File size: 38.  File blocks:
    12
    File contents:
    This is the spring of our discontent.\a

    === tests the performance of the Nachos file system ===
    Starting file system performance test:
    Ticks: total 1070, idle 1000, system 70, user 0
    Disk I/O: reads 2, writes 0
    Console I/O: reads 0, writes 0
    Paging: faults 0
    Network I/O: packets received 0, sent 0
    Sequential write of 50000 byte file, in 10 byte chunks
    Perf test: unable to write TestFile
    Sequential read of 50000 byte file, in 10 byte chunks
    Perf test: unable to read TestFile
    Ticks: total 114520, idle 113790, system 730, user 0
    Disk I/O: reads 17, writes 7
    Console I/O: reads 0, writes 0
    Paging: faults 0
    Network I/O: packets received 0, sent 0
    ```

## I. Basic Operation of the File System

### Exercise 1: Trace code

> Read the following code, understand current Nachos file system
>
> * `code/filesys/filesys.h`
> * `code/filesys/filesys.cc`
> * `code/filesys/filehdr.h`
> * `code/filesys/filehdr.cc`
> * `code/filesys/directory.h`
> * `code/filesys/directory.cc`
> * `code/filesys/openfile.h`
> * `code/filesys/openfile.cc`
> * `code/userprog/bitmap.h`
> * `code/userprog/bitmap.cc`

```txt
Nachos File System

+-----------------------------------+
|             File User             |
+-----------------------------------+
| FileSystem | OpenFile | Directory |
+------------+----------+-----------+
|            File Header            |
+-----------------------------------+
|             SynchDisk             |
+-----------------------------------+
|                Disk               |
+-----------------------------------+
```

![Nachos Study Book Nachos File System Structure](NachosFileSystemStructure.jpg)

```txt
Disk Allocation Structure

+----+----+---------------------+
| 0# | 1# | Normal Storage Area |
+----+----+---------------------+
  |     |
  |    1#: Root directory's i-node
  |
 0#: System bitmap file's i-node
```

The first sector is define in `code/filesys/filesys.cc`.

```c
// Sectors containing the file headers for the bitmap of free sectors,
// and the directory of files.  These file headers are placed in well-known
// sectors, so that they can be located on boot-up.
#define FreeMapSector 		0
#define DirectorySector 	1
```

#### The Virtual Disk

> This is implemented in `code/machine/disk.h` and `code/machine/disk.cc`

TBD

#### Synchronize Disk

> This is implemented in `code/filesys/synchdisk.h` and `code/filesys/synchdisk.cc`.
>
> Provides synchronous access to the asynchronous physical disk, so that threads block until their requests have completed.

In current Nachos, there are some constraint to limit the access of dick

* There is only one Thread can access disk at the same time
* When sending disk access request, need to wait until the access end.

The `SynchDisk::ReadSector` and `SynchDisk::WriteSector` will return only once the data is actually read or written.

> There will be a improvement of [asynchronized access in Exercise 6](#Exercise-6:-Trace-code-(implementation-of-asynchronous-access))

#### Free Space Management -- Bitmap

> This is implemented in `code/userprog/bitmap.h` and `code/userprog/bitmap.cc`

Nachos use bitmap to *manage free space*. The unit of physical disk block is a disk sector (扇區).
The numbering starts form 0. And 0 in bitmap means free space.

Nachos will keep this bitmap as a *bitmap file*. This bitmap file records entire system disk sectors usage.
This bitmap file should be mutual exclusive access.

#### File System

> This is implemented in `code/filesys/filesys.h` and `code/filesys/filesys.cc`
>
> Top-level interface to the file system

Initialization (`FileSystem::FileSystem`). That is [formating](#Format).
If we are not formatting the disk, just open the files representing the bitmap and directory. These are left open while Nachos is running.

Main operations

* `FileSystem::Create` (UNIX creat)
    > Yes, this function’s name is missing an e. Ken Thompson, the creator of Unix, once joked that the missing letter was his largest regret in the design of Unix. (from Linux System Programming)
* `FileSystem::Open` (UNIX open)
* `FileSystem::Remove` (UNIX unlink)

Main attributes

* `OpenFile* freeMapFile`: Bit map of free disk blocks, represented as a file
* `OpenFile* directoryFile`: "Root" directory -- list of file names, represented as a file

> Stub File System
>
> If use the `FILESYS_STUB` flag, Nachos will use the UNIX file system calls instead of its own.
> In the previous lab it use this, but in this lab we will use `FILESYS` instead.

#### Format

Steps when formating the file system

1. Generate new bitmap and empty root directory
2. Generate bitmap's file header and roo directory's file header
3. ... TODO

If define `FILESYS_NEEDED`. When calling `(void) Initialize(argc, argv);` in `main()` in `code/threads/main.cc`.

In `code/threads/system.cc`. Here is how disk been format.

```c
void
Initialize(int argc, char **argv)
{
    ...

#ifdef FILESYS_NEEDED
    bool format = FALSE;	// format disk
#endif

    ...

#ifdef FILESYS_NEEDED
	if (!strcmp(*argv, "-f"))
	    format = TRUE;
#endif

#ifdef FILESYS
    synchDisk = new SynchDisk("DISK");
#endif

#ifdef FILESYS_NEEDED
    fileSystem = new FileSystem(format);
#endif

    ...

}
```

This is how the file `code/filesys/DISK` came from.

#### File Header

> This is implemented in `code/filesys/filehdr.h` and `code/filesys/filehdr.cc`.
>
> Manages the data structure representing the layout of a file's data on disk.
> **This is the NachOS equivalent of a UNIX i-node**.

Because current Nachos only support direct indexing, and once determine the file length it can't be changed.

When need an header here is the procedure:

1. `FileHeader header = new FileHeader;`
2. `header->FetchFrom(sector);`
3. do something
4. (if have modified) `header->WriteBack(sector);`
5. `del header;`

The header is preserve in `DISK` as a `char` stream. And load it back the same way.
The total size of a header is a single sector (i.e. `SectorSize` (128 bytes)).

For further explaination of [UNIX i-node](#unix-i-node)

#### Open File (File Access)

> This is implemented in `code/filesys/openfile.h` and `code/filesys/openfile.cc`.

#### Directory Structure

> This is implemented in `code/filesys/directory.h` and `code/filesys/directory.cc`

Current Nachos only has single-layer directory (i.e. root directory). And the size of directory is fixed.

### Exercise 2: Expand file attributes

> Add file description information. Example:
>
> * file type
> * create time
> * last access time
> * last modify time
> * path
> * etc.
>
> Try to remove the limit of file name length.

Because we only have one directory now, and keep file name in i-node is not quite proper.
Thus I'll add the `path` attribute when I need it.

And I'll seperate this exercise into two phases

1. Add additional file attributes
2. Remove limit of file name length

Previous trace code result of the file header see [here](#file-header)

#### UNIX i-node

UNIX keeps three times for each file:

* last modification ( mtime )
* last access ( atime )
* last inode modification ( ctime )
  * A file's ctime is changed when the file is modified or when something kept in the inode (number of hard links, owner, group, etc.) has been changed.

> * [A File's Inode Change (not "Creation"!) Time](https://docstore.mik.ua/orelly/unix/upt/ch21_06.htm)
> * [Where are filenames stored on a filesystem?](https://unix.stackexchange.com/questions/117325/where-are-filenames-stored-on-a-filesystem)

#### 1. Add additional file attributes

As we want to add the additional file attribute we need to modify the file hader (i.e. i-node in UNIX term) in `code/filesys/filehdr.h`

Because the header is loaded from `DISK` only when it is needed. And the size is fixed in a sector so we shouldn't put too many (redundant) information in it.

But sometimes we need more information, but these information can be dynamically assign right when the header is opened. Thus I seperate a header in two part. Data part and In-core part.

The data part is exactly 128 bytes and will be store in the `DISK`.
Threre is a macro called `NumDirect`. The calculation of it is simply minus the header informations. And see how many space left for the direct disk indexing part. (i.e. the sector numbers where the data are)

The in-core part is stored in memory. And won't be store in `DISK`. But sometime we need additional temporary information to do something.

```txt
The visual FileHeader attributes

+-----------------------------------------+
|           FileHeader (i-node)           |
+-----------------------------------------+
|   Disk Part  | numBytes (int)           |
+              +--------------------------+
|   (total     | numSectors (int)         |
+  128 bytes)  +--------------------------+
|              | fileType (string)        |
+              +--------------------------+
|              | createdTime (string)     |
+              +--------------------------+
|              | modifiedTime (string)    |
+              +--------------------------+
|              | lastVisitedTime (string) |
+              +--------------------------+
|              | dataSectors[NumDirect]   | => size = 128 - others;
+--------------+--------------------------+    mapping entries = size/sizeof(int)
| In-core Part | headerSector             |
+--------------+--------------------------+
```

In order to get the current time and the file extension, I've made the following utility/helper functions.
(declare in `code/filesys/filehdr.h` and define in `code/filesys/filehdr.cc`)

```cpp
//----------------------------------------------------------------------
// getFileExtension
//    Extract the file name to get the extension. If the file name don't
//    have extension then return empty string. 
//
//      e.g. test.haha.pdf => "pdf"
//      e.g. test.txt      => txt
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
```

> I was tried to store the `struct tm` (or its pointer) in Header file. But the content of time will decode as random code (not sure why).
> So I compromise to store the time as string. (string length is 25 (include `\0` char) `echo "Sat May 11 22:37:41 2019" | wc` => `1       5      25`)
> Here is some notes about how to use `time.h`.
>
> But in `code/machine/sysdep.cc` there is the include of `time.h` but I'm not sure why it won't work as `string.h` (that we don't need to include it to use its function)
>
> ```cpp
> extern "C" {
> ...
> #include <sys/time.h>
> ...
> #ifdef HOST_i386
> ...
> #include <sys/time.h>
> ...
> #endif
> #ifdef HOST_SPARC
> ...
> #include <sys/time.h>
> #endif
>
> ...
> ```

#### 1-1. header structure

The calculation of the number of direct sector mapping (`NumDirect`) is

```cpp
// Disk part
#define NumOfIntHeaderInfo 2
#define NumOfTimeHeaderInfo 3
#define LengthOfTimeHeaderStr 26 // 25 + 1 ('/0')
#define MaxExtLength 5           // 4  + 1 ('/0')
#define LengthOfAllString MaxExtLength + NumOfTimeHeaderInfo*LengthOfTimeHeaderStr

#define NumDirect 	((SectorSize - (NumOfIntHeaderInfo*sizeof(int) + LengthOfAllString*sizeof(char))) / sizeof(int))
```

There will be "9" left for the mapping part in current FileHeader structure. (original will be "30").

Here is the structure of my FileHeader currently. (in `code/filesys/filehdr.h`)

```cpp
class FileHeader {
  private:
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
    int dataSectors[NumDirect]; // Disk sector numbers for each data
                                // block in the file
    // ======================== In-core Part ======================== //
    // This will be assign value when the file is open!
    int headerSector; // Because when we OpenFile, we need to update the header information
                      // but the sector message is only exist when create the OpenFile object
                      // some how we need to know which sector to write back
}
```

And I've also define some public function to manipulate them.

```cpp
class FileHeader {
  public:

    ...

    // Lab5: additional file attributes
    void HeaderCreateInit(char* ext); // Initialize all header message for creation
    // Disk part
    void setFileType(char* ext) { strcmp(ext, "") ? strcpy(fileType, ext) : strcpy(fileType, "None"); }
    void setCreateTime(char* t) { strcpy(createdTime, t); }
    void setModifyTime(char* t) { strcpy(modifiedTime, t); }
    void setVisitTime(char* t) { strcpy(lastVisitedTime, t); }
    // In-core part
    void setHeaderSector(int sector) { headerSector = sector; }
    int getHeaderSector() { return headerSector; }
}
```

The `FileHeader::HeaderCreateInit` is used when we first create the FileHeader.

```cpp
//----------------------------------------------------------------------
// FileHeader::HeaderCreateInit
//  Set the file type, time informations and other attribute.
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
```

#### 1.2 init and update the FileHeader

Last part of the last paragraph shows the init function.

This will be used for bitmap and directory file header (in `FileSystem::FileSystem` when formating).
Or for normal file header (in `FileSystem::Create` when create a file in Nachos disk).
Both are in file `code/filesys/filesys.cc` like this.

```c
FileSystem::FileSystem(bool format)
{
    if (format) {

        ...

        FileHeader *mapHdr = new FileHeader;
        mapHdr->HeaderCreateInit("BMap");

        FileHeader *dirHdr = new FileHeader;
        dirHdr->HeaderCreateInit("DirH");

        ...
    }
}

bool
FileSystem::Create(char *name, int initialSize)
{
    FileHeader *hdr;
    if (directory->Find(name) != -1)
        ...
    else
    {
        ...

        if (sector == -1)
            ...
        else if (!directory->Add(name, sector))
            ...
        else
        {
            hdr = new FileHeader;
            if (!hdr->Allocate(freeMap, initialSize))
                ...
            else
            {
                success = TRUE;
                hdr->HeaderCreateInit(getFileExtension(name)); // Lab5: additional file attributes

                ...
            }
            delete hdr;
```

Why we need the in-core part attriubte `headerSector` is because we need to update FileHeader when we open it.

At the start and end of the open file. (in `code/filesys/openfile.cc`)

```cpp
OpenFile::OpenFile(int sector)
{
    hdr = new FileHeader;
    hdr->FetchFrom(sector);
    hdr->setHeaderSector(sector); // Necessary, because we need to update
                                  // FileHeader(i-node) later on.
}

OpenFile::~OpenFile()
{
    hdr->WriteBack(hdr->getHeaderSector()); // Update the header info
    delete hdr;
}
```

And update the time attributes while read and write. (because the function `OpenFile::Read` and `OpenFile::Write` will call `OpenFile::ReadAt` and `OpenFile::WriteAt`, so just implement in two of them)

```cpp
int
OpenFile::ReadAt(char *into, int numBytes, int position)
{
    ...

    // Lab5: file header info update
    hdr->setVisitTime(getCurrentTime());
}

int
OpenFile::WriteAt(char *from, int numBytes, int position)
{
    ...

    // Lab5: file header info update
    hdr->setVisitTime(getCurrentTime());
    hdr->setModifyTime(getCurrentTime());
}
```

**Test the result**:

Because the script is running too fast, so I've add `sleep 1` in the test script `code/filesys/test/test_exercise_1-1.sh` before the line `./nachos -Q -p small`.
And also add `.txt` extension to the file `small`.

```sh
#!/bin/sh
# goto filesys/ in docker
cd /nachos/nachos-3.4/code/filesys

echo "=== copies file \"small\" from UNIX to Nachos (and add extension) ==="
./nachos -Q -cp test/small small.txt
sleep 1 # to observe the modification time change
echo "=== print the content of file \"small\" ==="
./nachos -Q -p small.txt
echo "=== prints the contents of the entire file system ==="
./nachos -Q -D
```

And this is the result of the `small` file header content.

```txt
Name: small, Sector: 11
------------ FileHeader contents -------------
        File type: txt
        Created: Sun May 12 03:06:50 2019
        Modified: Sun May 12 03:06:50 2019
        Last visited: Sun May 12 03:06:51 2019
File size: 38.  File blocks:
12
File contents:
This is the spring of our discontent.\a
----------------------------------------------
```

As you can see, the "Last visited" time is later than "Modified" time one second.

#### 2. Remove limit of file name length

```c
#define MaxFileSize (NumDirect * SectorSize)
```

### Exercise 3: Expand file length (size)

> Modify the direct indexing to indirect indexing. To break through the limit of 4KB file length.

### Exercise 4: Implement multi-level file directory

### Exercise 5: Dynamic allocate file length

> Modify the *create* and the *write* file operation to match the requirement.

## II. Synchronization and Mutual Exclusion of File Access

### Exercise 6: Trace code (implementation of asynchronous access)

> Read the asynchronous disk related code, understand how current Nachos asynchronous access disk.
>
> * `code/filesys/synchdisk.h`
> * `code/filesys/synchdisk.cc`
>
> Use this principle, implement `Class SynchConsole` based on `Class Console`

### Exercise 7: Implement synchronization and mutual exclusion mechnism of the file system

> 1. A file can be accessed by multiple thread at the same time. And for each thread which opens the file will own a current file access position and will not affect other threads
> 2. For all file system operation must be *atomic* and *sequential*.
>     * Example, when a thread is modifying a file, and another thread is read that file, then it should be either (with no exception)
>       * the reader thread load modified file
>       * the reader thread load the original file
> 3. When a thread want to delete a file, and other threads are accessing that file, must guarantee all the thread have closed the file. (i.e. you shouldn't really delete the file once a thread is using it.)

## Challenge

### Challenge 1: Performance optimization

> Example:
>
> 1. To improve the time of the HDD to search the file. Put the single file data block on the same disk path.
> 2. Use cache to reduce the access time.
>     * delay write
>     * pre-load

### Challenge 2: Implement pipe Mechanism

> Redirect openfile I/O method, such that the previous process can output the result to pipe, and the next process can get the data through pipe to console. (e.g. `ps aux | grep haha`)

## Trouble Shooting

### The file system flag don't work problem

Because in the `code/filesys/Makefile` it define the `THREADS` flag.

In the `code/threads/main.cc`, it will consume all the input flag/arguments...

Thus I add the `TEST_FILESYS` flag and modify the original `#ifdef THREADS` to `#if THREADS && !TEST_FILESYS`

> This bug waste me so much times = ="

### time.h

* [gmtime](http://www.cplusplus.com/reference/ctime/gmtime/): Convert time_t to tm as UTC time
* [asctime](http://www.cplusplus.com/reference/ctime/asctime/): Convert tm structure to string

tm structure

```c
struct tm {
   int tm_sec;         /* seconds,  range 0 to 59          */
   int tm_min;         /* minutes, range 0 to 59           */
   int tm_hour;        /* hours, range 0 to 23             */
   int tm_mday;        /* day of the month, range 1 to 31  */
   int tm_mon;         /* month, range 0 to 11             */
   int tm_year;        /* The number of years since 1900   */
   int tm_wday;        /* day of the week, range 0 to 6    */
   int tm_yday;        /* day in the year, range 0 to 365  */
   int tm_isdst;       /* daylight saving time             */
};
```

#### Comvert string back to time_t

* [Stackoverflow - How to convert a string variable containing time to time_t type in c++?](https://stackoverflow.com/questions/11213326/how-to-convert-a-string-variable-containing-time-to-time-t-type-in-c)

### Store class object using char string

### The time in docker container is not equal to system time

## TODO

* [ ] Fill up more detail in Exercise 1

## Resources

* [Nachos Filesystem](https://users.cs.duke.edu/~narten/110/nachos/main/node22.html)
  * SynchDisk
  * FileSystem Object
  * OpenFile Object
  * File System Physical Representation
    * File Header
    * Directories
    * Putting It All Together
* [Stackoverflow - Getting file extension in C](https://stackoverflow.com/questions/5309471/getting-file-extension-in-c)
* [**Slides - Nachos File System**](https://acm.sjtu.edu.cn/w/images/1/14/Nachos-p5-xjia.pdf) (this is Java version but with some important concept)

### Example

* [nachos Lab5實習報告](https://wenku.baidu.com/view/04382358f6ec4afe04a1b0717fd5360cbb1a8d40.html)

Other

* [Tips for NACHOS Assignment 4](https://cs.uwaterloo.ca/~brecht/courses/4321/assign4.html)
* [CSDN - 操作系統課設實驗五---Nachos文件系統擴展](https://blog.csdn.net/zekdot/article/details/83627721)
* [CSDN - nachos實驗——文件系統實現](https://blog.csdn.net/saber_jk/article/details/80922854)
