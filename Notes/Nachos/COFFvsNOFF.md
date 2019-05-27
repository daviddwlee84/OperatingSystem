# COFF vs NOFF

## COFF Common Object File Format

> Code references
>
> * `code/bin/coff.h`

### ELF

> Linux: ELF; Windows: PE (exe, dll...)

* ELF header
* Segment header table (required for executables)
* `.text` section
* `.rodata` section
* `.data` section
* `.bss` section
* `.symtab` section
* `.rel.txt` section
* `.rel.data` section
* `.debug` section
* Section header table

## NOFF Nachos Object Code Format

> Code references
>
> * `code/bin/noff.h`

* NOFF header
* `.text` section
* `.data` section
* `.bss` section

## Section to Address Space

> Code references
>
> * `AddrSpace` Constructor in `userprog/addrspace.cc`

* `.data` section => Data section
* `.text` section => Code section
