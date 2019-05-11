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

    ```sh
    #!/bin/sh
    # goto filesys/ in docker
    cd /nachos/nachos-3.4/code/filesys

    ./nachos -f # format the DISK
    ./nachos -cp test/big big # copies a file from UNIX to Nachos
    # Files: big
    ./nachos -cp test/small small
    # Files: big, small
    ./nachos -l # lists the contents of the Nachos directory
    # big
    # small
    ./nachos -r big # remove the file "big" from Nachos
    ./nachos -D # prints the contents of the entire file system 
    ./nachos -t # tests the performance of the Nachos file system
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

Initialization (`FileSystem::FileSystem`)

* When formating the file system
    1. Generate new bitmap and empty root directory
    2. Generate bitmap's file header and roo directory's file header
    3. ... TODO

Main operations

* `FileSystem::Create`
* `FileSystem::Open`
* `FileSystem::Remove`

> Stub File System
>
> If use the `FILESYS_STUB` flag, Nachos will use the UNIX file system calls instead of its own.
> In the previous lab it use this, but in this lab we will use `FILESYS` instead.

#### File Header

> This is implemented in `code/filesys/filehdr.h` and `code/filesys/filehdr.cc`.
>
> Manages the data structure representing the layout of a file's data on disk.
> **This is the NachOS equivalent of a UNIX i-node**.

Because current Nachos only support direct indexing, and once determine the file length it can't be changed.

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

#### Add additional file attributes

As we want to add the additional file attribute we need to modify the file hader (i.e. i-node in UNIX term) in `code/filesys/filehdr.h`

#### Remove limit of file name length

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

### Example

* [nachos Lab5實習報告](https://wenku.baidu.com/view/04382358f6ec4afe04a1b0717fd5360cbb1a8d40.html)
* [CSDN - 操作系統課設實驗五---Nachos文件系統擴展](https://blog.csdn.net/zekdot/article/details/83627721)
