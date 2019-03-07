/*
 Copyright (c) 1992-1993 The Regents of the University of California.
 All rights reserved.  See copyright.h for copyright notice and limitation 
 of liability and disclaimer of warranty provisions.
 */

#define MAIN
#include "copyright.h"
#undef MAIN

/*
 * OUT.C
 * Looking at a.out formats.
 *
 * First task:
 * Look at mips COFF stuff:
 * Print out the contents of a file and do the following:
 *    For data, print the value and give relocation information
 *    For code, disassemble and give relocation information
 */

#include <filehdr.h>
#include <aouthdr.h>
#include <scnhdr.h>
#include <reloc.h>
#include <syms.h>
#include <stdio.h>

#define read_struct(f,s) (fread(&s,sizeof(s),1,f)==1)

#define MAXRELOCS 1000


#define MAXDATA 10000

struct data {
  long data[MAXDATA];
  struct reloc reloc[MAXRELOCS];
  int length;
  int relocs;
};

#define MAXSCNS 10
#define MAXSYMS 300
#define MAXSSPACE 20000

struct filehdr filehdr;
struct aouthdr aouthdr;
struct scnhdr scnhdr[MAXSCNS];
struct data section[MAXSCNS];
HDRR symhdr;
EXTR symbols[MAXSYMS];
char sspace[20000];

char *symbol_type[] = {
  "Nil", "Global", "Static", "Param", "Local", "Label", "Proc", "Block",
  "End", "Member", "Type", "File", "Register", "Forward", "StaticProc",
  "Constant" };

char *storage_class[] = {
  "Nil", "Text", "Data", "Bss", "Register", "Abs", "Undefined", "CdbLocal",
  "Bits", "CdbSystem", "RegImage", "Info", "UserStruct", "SData", "SBss",
  "RData", "Var", "Common", "SCommon", "VarRegister", "Variant", "SUndefined",
  "Init" };
	  
main(argc,argv)
int argc;
char *argv[];
{
  char *filename = "a.out";
  FILE *f;
  int i;
  long l;
/*   EXTR filesym; */
  char buf[100];

  if (argc == 2) filename = argv[1];
  if ((f = fopen(filename,"r")) == NULL) {
    printf("out: could not open %s\n",filename);
    perror("out");
    exit(1);
  }
  if (!read_struct(f,filehdr) ||
      !read_struct(f,aouthdr) ||
      filehdr.f_magic != MIPSELMAGIC) {
    printf("out: %s is not a MIPS Little-Endian COFF object file\n",filename);
    exit(1);
  }
  if (filehdr.f_nscns > MAXSCNS) {
    printf("out: Too many COFF sections.\n");
    exit(1);
  }
  for (i=0; i < filehdr.f_nscns; ++i) {
    read_struct(f,scnhdr[i]);
    if (scnhdr[i].s_size > MAXDATA*sizeof(long) &&
	scnhdr[i].s_scnptr != 0 ||
	scnhdr[i].s_nreloc > MAXRELOCS) {
      printf("section %s is too big.\n",scnhdr[i].s_name);
      exit(1);
    }
  }
  for (i=0; i < filehdr.f_nscns; ++i) {
    if (scnhdr[i].s_scnptr != 0) {
      section[i].length = scnhdr[i].s_size/4;
      fseek(f,scnhdr[i].s_scnptr,0);
      fread(section[i].data,sizeof(long),section[i].length,f);
      section[i].relocs = scnhdr[i].s_nreloc;
      fseek(f,scnhdr[i].s_relptr,0);
      fread(section[i].reloc,sizeof(struct reloc),section[i].relocs,f);
    } else {
      section[i].length = 0;
    }
  }
  fseek(f,filehdr.f_symptr,0);
  read_struct(f,symhdr);
  if (symhdr.iextMax > MAXSYMS) {
    printf("too many symbols to store.\n");
  }
  fseek(f,symhdr.cbExtOffset,0);
  for (i=0; i < MAXSYMS && i<symhdr.iextMax; ++i) {
    read_struct(f,symbols[i]);
  }
  if (symhdr.issExtMax > MAXSSPACE) {
    printf("too large a string space.\n");
    exit(1);
  }
  fseek(f,symhdr.cbSsExtOffset,0);
  fread(sspace,1,symhdr.issExtMax,f);

  for (i=0; i<filehdr.f_nscns; ++i) {
    print_section(i);
  }

  printf("External Symbols:\nValue\t Type\t\tStorage Class\tName\n");
  for (i=0; i < MAXSYMS && i < symhdr.iextMax; ++i) {
    SYMR *sym = &symbols[i].asym;
    if (sym->sc == scUndefined) myprintf("\t ");
    else myprintf("%08x ",sym->value);
    myprintf("%s",symbol_type[sym->st]);
    mytab(25);
    myprintf("%s",storage_class[sym->sc]);
    mytab(41);
    myprintf("%s\n",&sspace[sym->iss]);
  }
  return 0;
}

static column = 1;
static FILE *outfile = stdout;

#include <varargs.h>
/*VARARGS0*/
myprintf(va_alist)
va_dcl
{
  va_list ap;
  char *form;
  char buf[100];

  va_start(ap);
  form = va_arg(ap,char *);
  vsprintf(buf,form,ap);
  va_end(ap);

  fputs(buf,outfile);

  for (form = buf; *form != '\0'; ++form) {
    if (*form == '\n') column = 1;
    else if (*form == '\t') column = ((column + 7)&~7)+1;
    else column += 1;
  }
}

mytab(n)
int n;
{
  while (column < n) {
    fputc(' ',outfile);
    ++column;
  }
  return column == n;
}  

mysetfile(f)
FILE *f;
{
  outfile = f;
}

#define printf myprintf
#include "d.c"

print_section(i)
int i;
{
  int j,k;
  int is_text;
  long pc;
  long word;
  char *s;

  printf("Section: %s\t%d/%d\n",scnhdr[i].s_name,
	 scnhdr[i].s_size,section[i].relocs);
  is_text =  (strncmp(scnhdr[i].s_name,".text",5) == 0);

  for (j=0; j < section[i].length; ++j) {
    pc = scnhdr[i].s_vaddr+j*4;
    word = section[i].data[j];
    if (is_text) {
      dump_ascii(word,pc);
    } else {
      printf("%08x: %08x  ", pc,word);
      s = (char *)&word;
      for (k=0;k<4;++k) {
	if (s[k] >= ' ' && s[k] < 127) printf("%c",s[k]);
	else printf(".");
      }
      printf("\t%d",word);
    }
    print_reloc(pc,i,j);
  }
}

char *section_name[] = {
  "(null)", ".text", ".rdata", ".data", ".sdata", ".sbss", ".bss",
  ".init", ".lit8", ".lit4"
};

char *reloc_type[] = {
  "abs", "16", "32", "26", "hi16", "lo16", "gpdata", "gplit"
};

print_reloc(vaddr,i,j)
int i,j;
{
  int k;
  struct reloc *rp;
  for (k=0; k < section[i].relocs; ++k) {
    rp = &section[i].reloc[k];
    if (vaddr == rp->r_vaddr) {
      mytab(57);
      if (rp->r_extern) {
	if (rp->r_symndx >= MAXSYMS) {
	  printf("sym $%d",rp->r_symndx);
	} else {
	  printf("\"%s\"",&sspace[symbols[rp->r_symndx].asym.iss]);
	}
      } else {
	printf("%s",section_name[rp->r_symndx]);
      }
      printf(" %s",reloc_type[rp->r_type]);
      break;
    }
  }
  printf("\n");
}
