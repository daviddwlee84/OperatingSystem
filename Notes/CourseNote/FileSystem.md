# File System

## Basis of File System

### What is file and file system

### Logic Structure of File

The inner structure of a file is decide by how the user access it. E.g. different file format. (*.txt, *.pdf, *.mp3, etc.)

The file system only two way to access it.

1. Sequential access 流式
2. 記錄式
   * i.e. [Directory File](#Directory-File)

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



Disk Access

Steps:

1. 
2. 
3. 

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

* File Directory 文件目錄
* Directory Entry 目錄項
* Directory File 目錄文件

#### File Directory

* Manage all the files information at once!
* Form all the management information (i.e. FCBs) together

#### Directory Entry

* The basic unit to form a File Directory
* The entry "can" be FCB

#### Directory File

* Only one in a file system
* A special file type that the content is consist with the Directory Entries.

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

#### Unix File System Arrangement

#### MS DOS File System (FAT Structure) Arrangement

> DOS stands for Disk Operating System; FAT stands for File Allocation Table 文件分配表

#### NTFS File System Arrangement

### File Physical Structure (Allocation Method)

* Sequential/Continuous Allocation Structure
  ![Figure 12.5 - Contiguous allocation of disk space.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter12/12_05_ContiguousAllocation.jpg)
  * Pros and Cons
    * Pros
    * Cons
* Linked Allocation Structure
  ![Figure 12.6 - Linked allocation of disk space.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter12/12_06_LinkedAllocation.jpg)
  * Pros and Cons
    * Pros
    * Cons
  * e.g. [FAT](#Linked-Structure-in-FAT)
* Indexed Allocation Structure
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

![Figure 12.7 File-allocation table.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter12/12_07_FAT_Table.jpg)

#### Index Structure in Unix

![Figure 12.9 - The UNIX inode.](https://www.cs.uic.edu/~jbell/CourseNotes/OperatingSystems/images/Chapter12/12_09_UNIX_inode.jpg)

### Directory File Structure

> The Directory Entry is FCB

* 散列表
* B Tree or B+ Tree
  * e.g. NTFS

### File Directory Retrieval

### Improvement of Directory File

#### Faster Retrieval Time

e.g. Unix i-node (index node)

## File System Instance

### UNIX

* FCB = Directory Entry + i-node

### FAT16

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

### Book

Operating System Concept 9ed.

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
