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
