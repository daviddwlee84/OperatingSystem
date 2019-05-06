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

* Tree-like Structure 樹形結構
* Array-like Structure 一級結構

#### Tree-like Structure

* Root dir
  * User dir
    * User subdir
      * User file

Route/File Path (File Name)

* Relative Path: Start from root directory
* Definite Path: Start from current directory

Operations

* create, delete
* opendir, closedir
* readdir, rename
* link, unlink

Each directory is a Directory File. The entroy of the Directory File can be Directory File or normal File.

#### Array-like Structure

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

#### FAT File System Arrangement

> FAT stands for File Allocation Table 文件分配表

#### NTFS File System Arrangement

### File Physical Structure

* Sequential/Continuous Structure
  * Pros and Cons
    * Pros
    * Cons
* Linked Structure
  * Pros and Cons
    * Pros
    * Cons
  * e.g. FAT
* Index Structure
  * Pros and Cons
    * Pros
    * Cons
  * e.g. Unix

#### Linked Structure in FAT

#### Index Structure in Unix

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
