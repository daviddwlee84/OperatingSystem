# File System

## Basis of File System

### What is file and file system

### Logic Structure of File

The inner structure of a file is decide by how the user access it. E.g. different file format. (*.txt, *.pdf, *.mp3, etc.)

The file system only two way to access it. (the [File structure](https://en.wikipedia.org/wiki/File_format#File_structure))

1. Sequential access (Unstructured formats) 流式
2. Structured formats 記錄式
   1. Chunk-based formats
   2. Directory-based formats
      * i.e. [Directory File](#Directory-File)

> * 流式文件(無結構文件)
> * 記錄式文件(有結構文件)

### Accessing File

* Sequential Access
* Random Access
  * e.g. seek in Unix

### Storing Medium (Physical vs. Logical)

* Classical Storing Medium
  * HDD
  * SSD
  * Flash
  * ...
* **Block**(塊) in Unix-like System or **Cluster**(簇) in Windows System
* Sector 扇區

#### HDD Structure

![https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter10/10_01_DiskMechanism.jpg](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter10/10_01_DiskMechanism.jpg)

Components

* **Arm** 磁臂
  * Arm Assembly
  * Read-Write Head 磁頭
* **Track** 磁道
* **Sector** 扇區
* Cylinder 磁柱
* Platter 盤片(面)

Disk Access

Steps:

1. Move the disk arm
2. Find the disk track
3. Find the platter
4. Spin
5. Read the sector with disk head

Access Time = Seek Time + Rotational Delay + Transfer Time

* Seek Time: move the disk arm and settle on a cylinder (5-15 ms)
* Rotational Delay: (8 ms for full rotation at 7200 RPM) => average delay = 4ms
* Transfer Time: (1 ms for an 8KB block at 8MB/s)

> Bandwidth utilization is less than 50% for any noncontiguous access at a block grain

### File Attributes

#### File Control Block (FCB)

![Figure 12.2 - A typical file-control block.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter12/12_02_FileControlBlock.jpg)

#### Common Attributes

* Filename
* File number
* ...
* Creater
* Owner
* File type
* File size
* Create time
* Last update time
* Last access time
* ...

### File Operations

* create
* delete
* open
* close
* read
* write
* append
* seek
* get attributes
* set attributes
* rename

### File Directory, Directory Entry and Directory File

* File Directory 文件目錄: An abstract existance
  * Manage all the files information at once!
  * Form all the management information (i.e. FCBs) together
* Directory Entry 目錄項
  * The basic unit to form a File Directory
  * The entry "can" be FCB
* Directory File 目錄文件
  * Only one in a file system
  * A special file type that the content is consist with the Directory Entries.

Basically in the regular directory file will at least have these two directory entry

* `.`: current directory sector (or i-node number)
* `..`: parent directory sector (or i-node number)
  * for root directory is itself

### Directory Structure

* Hierarchical Directory Structure 樹形結構
* Single-Level Directory Structure 一級結構

#### Hierarchical Directory Structure

* Root dir
  * User dir
    * User subdir
      * User file

Route/File Path (File Name)

* Relative Path: Start from root directory
* Absolute Path: Start from current directory

Operations

* create, delete
* opendir, closedir
* readdir, rename
* link, unlink

Each directory is a Directory File. The entroy of the Directory File can be Directory File or normal File.

#### Single-Level Directory Structure

## Implememtation of File System

* Partition 磁盤分區: Seperate disk into several individual part.
* Volume 文件卷: Single or multiple continuous sector, data block.
  * Block 塊
  * Cluster 簇
* Format 格式化: Create and initialization the file system

### Content on dick

* 引導區
* 分區(卷)信息
* 目錄結構(目錄文件)
* 用戶文件

#### Unix File System Layout

![UNIX Layout](https://gcallah.github.io/OperatingSystems/graphics/FileSystemLayout.PNG)

Entire Disk

* **MBR (Master Boot Record)** 主引導記錄
  * is used to boot the computer
* **Partition Table** 分區表
  * Partition table is present at the end of MBR. This table gives the starting and ending addresses of each partition.
* **Disk Partitions**

One of the Disk Partitions

* **Boot Block** 引導記錄
  * When the computer is booted, the BIOS reads in and executes the MBR. The first thing the MBR program does is locate the active partition, read in its first block, which is called the boot block, and execute it. The program in the boot block loads the operating system contained in that partition. Every partition contains a boot block at the beginning though it does not contain a bootable operating system.
* **Super Block** 超級數據塊
  * It contains all the key parameters about the file system and is read into memory when the computer is booted or the file system is first touched.
* **Free Space Management** 空閒區管理
  * e.g. put the bitmaps or linked list data structure
* **I-nodes Area** (sector 1)
  * Arrange all the i-nodes data structure
  * the metadata e.g. file attributes
* **Root Directory** (sector 2)
  * literary, put the root directory file
* **Files and Directories** (sector 3, 4, ...)
  * put file or directory file

#### MS DOS File System (FAT Structure) Layout

> DOS stands for Disk Operating System; FAT stands for File Allocation Table 文件分配表
>
> * [Wiki - File Allocation Table](https://en.wikipedia.org/wiki/File_Allocation_Table)
> * [Wiki - Design of the FAT file system](https://en.wikipedia.org/wiki/Design_of_the_FAT_file_system)

Regions

* Reserved sectors
  * **Boot Sector** 引導區 (sector 0)
  * FS Information Sector (FAT32 only)
  * More reserved sectors (optional)
* **FAT Region** 文件分配表 (sector 1, 2)
  * File Allocation Table #1
  * File Allocation Table #2 ... (optional)
* **Root Directory** Region (sector 3) (FAT12 and FAT16 only)
  * (in our course is sector 2, 3, so make sure write "sector" 2 for root directory)
* **Data Region**
  * Other files and directory files

#### NTFS File System Arrangement

Skip

### File Physical Structure (Allocation Method)

* **Sequential/Continuous Allocation** Structure: Each file is stored as a contiguous run of disk blocks
  ![Figure 12.5 - Contiguous allocation of disk space.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter12/12_05_ContiguousAllocation.jpg)
  * Pros and Cons
    * Pros
      * Simple to implement
      * The read performance is excellent because the entire file can be read from the disk in a single operation
    * Cons
      * Over the course of time the disk becomes **fragmented**
* **Linked Allocation** Structure
  ![Figure 12.6 - Linked allocation of disk space.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter12/12_06_LinkedAllocation.jpg)
  * Pros and Cons
    * Pros
    * Cons
  * e.g. [FAT](#Linked-Structure-in-FAT): Linked-List Allocation Using a Table in Memory
* **Indexed Allocation** Structure
  ![Figure 12.8 - Indexed allocation of disk space.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter12/12_08_IndexedAllocation.jpg)
  * Pros and Cons
    * Pros
    * Cons
  * e.g. [Unix](#Index-Structure-in-Unix)

| -                           | Continuous Structure | Linked Structure                                       | Indexed Structure            |
| --------------------------- | -------------------- | ------------------------------------------------------ | ---------------------------- |
| Dynamic Allocation          | Unable               | Able                                                   | Able                         |
| Sepcific file length Create | Yes                  | No                                                     | No                           |
| Well use free space         | No                   | Yes                                                    | Yes                          |
| Access Method               | Sequential access    | Sequential access (mainly) (FAT support Random access) | Sequential and Random access |
| Access Speed                | Fast                 | Slower                                                 | Fast                         |
| Classic System              | Disk Tape File       | MS-DOS                                                 | UNIX                         |

#### Linked Structure in FAT

[FAT Layout](#ms-dos-file-system-fat-structure-layout)

* block number
  * `> 0`: number of next block
  * `= -1`: last block of the list
  * `= 0`: empty block (initial state)

![Figure 12.7 File-allocation table.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter12/12_07_FAT_Table.jpg)

#### Index Structure in Unix

* 1~12 direct blocks
* 13 single indirect block
* 14 double indirect block
* 15 triple indirect block

[EXT Layout](#unix-file-system-layout)

![Figure 12.9 - The UNIX inode.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter12/12_09_UNIX_inode.jpg)

![i-nodes](https://gcallah.github.io/OperatingSystems/graphics/inode.png)

> Assume a sector size is 512 bytes and a single indirect block can store 256 ($2^8$) indices
>
> For a triple indirect structure can have the maximum file size:
>
> $$
> (12 + 2^8 + 2^{16} + 2^{24}) \times 512 \text{ bytes}
> $$

### Directory File Structure

> The Directory Entry is FCB

* Sequential
* Hash Table (哈希表, 雜湊表, 散列表)
  * e.g. key: file name; value: sector
* B Tree or B+ Tree
  * e.g. NTFS

### File Directory Retrieval

1. Use a filename to search directory entry (FCB) in a directory
2. According to the sector information (or i-node number) get from FCB to access the location of the file

### Improvement of Directory File

> Speed up the retrieval time

#### Faster Retrieval Time

e.g. Unix i-node (index node)

Seperate the directory entry (FCB) into two parts

* i-nodes
  * file attributes
  * file sector
* basic directory entry
  * file names

## File System Instance

### UNIX

* FCB = Directory Entry + i-node

### FAT16

## Free Space Management

### Data Structure for Free Space Management

#### Bitmap (Bit vector)

![bitmap](https://cdncontribute.geeksforgeeks.org/wp-content/uploads/1-265.png)

> can be represented by a bitmap of 16 bits as: 0000111000000110

#### Free Block Table

#### Free Block Linked List

![Free block list](https://cdncontribute.geeksforgeeks.org/wp-content/uploads/2-190.png)

**Grouping List Method** 成組鏈接法

## In-Memory File System Data Structure

![In-memory file-system structures. (a) File open. (b) File read](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter12/12_03_FileSystemStructures.jpg)

### Example of UNIX

1. System opened files table
   * Only one in a system
   * Store the opened files' FCB
2. User opened files table
   * Record in PCB (thus one for each process)
   * **File Descriptor** is its entry

## File System Operations Implementation

* Create
* Open
* Seek
* Read
* ...

## Sharing File

> A file used by multiple users or processes

### Link

* [Soft and Hard links in Unix/Linux](https://www.geeksforgeeks.org/soft-hard-links-unixlinux/)

Command

* `ls -l`: shows all the links with the link column shows number of links

#### Hard Link

* Each hard linked file is *assigned the same Inode value* as the original, therefore they reference the same physical file location.
* Hard links more flexible and remain linked even if the original or linked files are moved throughout the file system
* Links have actual file contents
* If original file is removed then the link will still show the content of the file
* We cannot create a hard link for a directory (to avoid recursive loops)

Command to create a hard link

```sh
ln [original filename] [link name]
```

#### Soft Link

* A soft link is similar to the file shortcut feature which is used in Windows
* Each soft linked file contains a *separate Inode value that points to the original file*
* As similar to hard links, any changes to the data in either file is reflected in the other. Soft links can be linked across different file systems, although if the original file is deleted or moved, the soft linked file will not work correctly (called hanging link)
* Soft Link contains the path for original file and not the contents
* Removing soft link doesn’t affect anything
* Removing original file, the link becomes “dangling” link which points to nonexistent file
* A soft link can link to a directory

Command to create a Soft link is:

```sh
ln -s [original filename] [link name]
```

### Mount

## File System Management

* Reliability
  * backup
* Consistency
  * system crash before write the data from memory to disk sector
    * that's why when you close the computer unnormally, system will scan and check the disk
  * Writing Method
    * write-through 通寫: immediate write back the modification in disk
      * e.g. FAT
    * lazy-write 延遲寫: accumulate and write back multiple things at once
      * write back cache
    * transaction log 可恢復寫
      * e.g. NTFS
* Security
  * Protection
    * File Protection
    * User Identity Verification
    * Access Control
    * UNIX file protection
      * seperate user
        * owner
        * group
        * other
      * operation authority
        * read (r)
        * write (w)
        * execution (x)
        * no authority (-)

Data Recovery

## File System Performance

Methods to Improvement Filesystem Performance

* FCB separation
  * e.g. UNIX i-node
* Disk fragment organization
* Cache
* Pre-load
* RAID
* ...

### Cache

### Pre-load

### Disk Scheduling

### RAID

Parallelism => High speed

Data Redundance (e.g. mirror or varification) => Fault tolerance

![RAID levels](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter10/10_11_RAID_Levels.jpg)

## Exercise

### System Arrangement

Consider a disk

* Size 2MB.
* Each block/cluster is 512 bytes

The disk has been formatted. And show the final state of the disk.

1. in `/` mkdir `A`
2. in `A` mkdir `B`
3. in `B` create `File1` (with 4 blocks/clusters)
4. in `/` mkdir `C`
5. in `/` mkdir `D`
6. in `C` mkdir `E`
7. in `E` create `File2` (with 16 blocks/clusters)
8. in `E` mkdir `F`
9. in `F` create `File3` (with 4 blocks/clusters)
10. in `F` create `File4` (with 4 blocks/clusters)

#### UNIX Arrangement

#### FAT16 Arrangement

## Resources

* [**Slides - File Systems and Disk Layout File Systems and Disk Layout**](http://db.cs.duke.edu/courses/spring01/cps110/slides/disk-fs.pdf) - Include Nachos
* [File System Implementation](https://gcallah.github.io/OperatingSystems/FileImplementation.html)
* [Wiki - Disk sector](https://en.wikipedia.org/wiki/Disk_sector)
* [Wiki - Data cluster](https://en.wikipedia.org/wiki/Data_cluster)
* [Wiki - Block (data storage)](https://en.wikipedia.org/wiki/Block_(data_storage))
* [Wiki - File system](https://en.wikipedia.org/wiki/File_system)
* [Operating System | Free space management](https://www.geeksforgeeks.org/operating-system-free-space-management/)

### Book

Operating System Concept 9ed.

* Ch10 Mass-Storage Structure
  * Ch10.1 Overview of Mass-Storage Structure
    * Ch10.1.1 Magnetic Disks
    * Ch10.1.2 Solid-State Disks
  * Ch10.2 Disk Structure
  * Ch10.4 Disk Scheduling
    * Ch10.4.1 FCFS Scheduling
    * Ch10.4.2 SSTF Scheduling (Shortest Seek Time First)
    * Ch10.4.3 SCAN Scheduling (a.k.a. the elevator algorithm)
    * Ch10.4.4 C-SCAN Scheduling (Circular-SCAN)
    * Ch10.4.5 LOOK Scheduling
  * Ch10.5 Disk Management
    * Ch10.5.1 Disk Formatting
    * Ch10.5.2 Boot Block
  * Ch10.6 Swap-Space Management
  * Ch10.7 RAID Structure
* Ch11 File-System Interface
  * Ch11.1 File Concept
    * Ch11.1.3 File Types
  * Ch11.2 Access Methods
    * Ch11.2.1 Sequential Access
    * Ch11.2.2 Direct Access
  * Ch11.3 Directory and Disk Structure
    * Ch11.3.2 Directory Overview
    * Ch11.3.3 Single-Level Directory
    * Ch11.3.5 Tree-Structured Directories
  * Ch11.6 Protection
    * Ch11.6.1 Types of Access
    * Ch11.6.2 Access Control
* Ch12 File-System Implementation
  * Ch12.2 File-System Implementation
  * Ch12.3 Directory Implementation
    * Ch12.3.1 Linear List
    * Ch12.3.2 Hash Table
  * **Ch12.4 Allocation Method**
    * Ch12.4.1 Contiguous Allocation
    * Ch12.4.2 Linked Allocation
    * Ch12.4.3 Indexed Allocation
  * Ch12.5 Free-Space Management
    * Ch12.5.1 Bit Vector
    * Ch12.5.2 Linked List
* Notes
  * [Ch10 Mass-Storage Structure](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/10_MassStorage.html)
  * [Ch11 File-System Interface](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/11_FileSystemInterface.html)
  * [Ch12 File-System Implementation](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/12_FileSystemImplementation.html)

Modern Operating System 4ed. Part 4 File System

* Ch4.1 Files
  * Ch4.1.1 File Naming
  * Ch4.1.2 File Structure
  * Ch4.1.3 File Types
  * Ch4.1.4 File Access
  * Ch4.1.5 File Attributes
  * Ch4.1.6 File Operations
* Ch4.2 Directories
  * Ch4.2.1 Single-Level Directory System
  * Ch4.2.2 Hierarchical Directory System
  * Ch4.2.3 Path Names - absolute, relative path name
  * Ch4.2.4 Directory Operations
* Ch4.3 File-System Implementation
  * Ch4.3.1 File-System Layout - MBR
  * Ch4.3.2 Implementing Files
    * Contiguous Allocation
    * Linked-List Allocation
    * Linked-List Allocation Using a Table in Memory - i.e. FAT
    * I-nodes
  * Ch4.3.3 Implementing Directories
* Ch4.4 File-System Management and Optimization
  * Ch4.4.1 Disk-Space Management
    * Block Size
    * Keeping Track of Free BLocks
    * Disk Quotas
* Ch4.5 Example File System
  * Ch4.5.1 The MS-DOS File System
  * Ch4.5.2 The UNIX V7 File System

### UNIX File System

* [Wiki - Unix File System](https://en.wikipedia.org/wiki/Unix_File_System) For UFS, a specific file system used by many Unix and Unix-like operating systems, see Unix File System.
* [Wiki - inode](https://en.wikipedia.org/wiki/Inode)
* [The Second Extended File system (EXT2)](http://www.science.unitn.it/~fiorella/guidelinux/tlk/node95.html)

> * [Wiki - Filesystem Hierarchy Standard](https://en.wikipedia.org/wiki/Filesystem_Hierarchy_Standard)
> * [GeeksforGeeks - Operating System | Unix File System](https://www.geeksforgeeks.org/operating-system-unix-file-system/)
> * [Wiki - Unix filesystem](https://en.wikipedia.org/wiki/Unix_filesystem)
> * [5. The UNIX Filesystem](http://web.deu.edu.tr/doc/oreily/networking/puis/ch05_01.htm)
> * [Unix / Linux - File System Basics](https://www.tutorialspoint.com/unix/unix-file-system.htm)
> * [The Unix File System](http://homepages.uc.edu/~thomam/Intro_Unix_Text/File_System.html)
> * [Unix File System and Directory Structure](https://www.gotothings.com/unix/unix-file-system-and-directory-structure.htm)
