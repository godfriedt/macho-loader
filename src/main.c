
#include "load.h"

int main(int argc, char* argv[]) {
    int fd = open(argv[1], O_RDONLY);

    struct stat st;
    fstat(fd, &st);
    int* macho = (int*) mmap(0, st.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (macho == MAP_FAILED) {
        perror("mmap");
    }
    int* beginning = macho;

    // load shellcode POC
    macho += 9; // first load segment offset
    int p0_size;
    memcpy(&p0_size, macho, 4);
    macho += p0_size / 4; // skip to next load segment
    macho--;
    macho += 18; // go to __text section
    macho += 10; // go to code size
    uint64_t sc_size;
    memcpy(&sc_size, macho, 8); // set code size
    macho += 2; // go to file offset of executable code
    uint32_t off;
    memcpy(&off, macho, 4);

    void* mem_ptr = mmap(0, sc_size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANON | MAP_JIT, -1, 0);
    if (mem_ptr == MAP_FAILED) {
        perror("mmap");
    }
    memcpy(mem_ptr, beginning + (off/4), sc_size); // map the executable code into memory
    mprotect(mem_ptr, sc_size, PROT_READ | PROT_EXEC);

    void (*func)() = mem_ptr;
    func();

    return 0;
}

//struct mach_header_64 {
//    uint32_t	magic;		/* mach magic number identifier */
//    int32_t	cputype;	/* cpu specifier */
//    int32_t	cpusubtype;	/* machine specifier */
//    uint32_t	filetype;	/* type of file */
//    uint32_t	ncmds;		/* number of load commands */
//    uint32_t	sizeofcmds;	/* the size of all the load commands */
//    uint32_t	flags;		/* flags */
//    uint32_t	reserved;	/* reserved */
//};

//struct load_command {
//    uint32_t cmd;		/* type of load command */
//    uint32_t cmdsize;	/* total size of command in bytes */
//};

//struct segment_command_64 { /* for 64-bit architectures */
//    uint32_t	cmd;		/* LC_SEGMENT_64 */
//    uint32_t	cmdsize;	/* includes sizeof section_64 structs */
//    char		segname[16];	/* segment name */
//    uint64_t	vmaddr;		/* memory address of this segment */
//    uint64_t	vmsize;		/* memory size of this segment */
//    uint64_t	fileoff;	/* file offset of this segment */
//    uint64_t	filesize;	/* amount to map from the file */
//    int32_t	maxprot;	/* maximum VM protection */
//    int32_t	initprot;	/* initial VM protection */
//    uint32_t	nsects;		/* number of sections in segment */
//    uint32_t	flags;		/* flags */
//};

//struct section_64 { /* for 64-bit architectures */
//    char		sectname[16];	/* name of this section */
//    char		segname[16];	/* segment this section goes in */
//    uint64_t	addr;		/* memory address of this section */
//    uint64_t	size;		/* size in bytes of this section */
//    uint32_t	offset;		/* file offset of this section */
//    uint32_t	align;		/* section alignment (power of 2) */
//    uint32_t	reloff;		/* file offset of relocation entries */
//    uint32_t	nreloc;		/* number of relocation entries */
//    uint32_t	flags;		/* flags (section type and attributes)*/
//    uint32_t	reserved1;	/* reserved (for offset or index) */
//    uint32_t	reserved2;	/* reserved (for count or sizeof) */
//    uint32_t	reserved3;	/* reserved */
//};

