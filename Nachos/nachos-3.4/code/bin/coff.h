/* coff.h
 *   Data structures that describe the MIPS COFF format.
 */

struct filehdr {
        unsigned short  f_magic;        /* magic number */
        unsigned short  f_nscns;        /* number of sections */
        long            f_timdat;       /* time & date stamp */
        long            f_symptr;       /* file pointer to symbolic header */
        long            f_nsyms;        /* sizeof(symbolic hdr) */
        unsigned short  f_opthdr;       /* sizeof(optional hdr) */
        unsigned short  f_flags;        /* flags */
      };

#define  MIPSELMAGIC    0x0162

#define OMAGIC  0407
#define SOMAGIC 0x0701

typedef struct aouthdr {
        short   magic;          /* see above                            */
        short   vstamp;         /* version stamp                        */
        long    tsize;          /* text size in bytes, padded to DW bdry*/
        long    dsize;          /* initialized data "  "                */
        long    bsize;          /* uninitialized data "   "             */
        long    entry;          /* entry pt.                            */
        long    text_start;     /* base of text used for this file      */
        long    data_start;     /* base of data used for this file      */
        long    bss_start;      /* base of bss used for this file       */
        long    gprmask;        /* general purpose register mask        */
        long    cprmask[4];     /* co-processor register masks          */
        long    gp_value;       /* the gp value used for this object    */
      } AOUTHDR;
#define AOUTHSZ sizeof(AOUTHDR)
 

struct scnhdr {
        char            s_name[8];      /* section name */
        long            s_paddr;        /* physical address, aliased s_nlib */
        long            s_vaddr;        /* virtual address */
        long            s_size;         /* section size */
        long            s_scnptr;       /* file ptr to raw data for section */
        long            s_relptr;       /* file ptr to relocation */
        long            s_lnnoptr;      /* file ptr to gp histogram */
        unsigned short  s_nreloc;       /* number of relocation entries */
        unsigned short  s_nlnno;        /* number of gp histogram entries */
        long            s_flags;        /* flags */
      };
 
