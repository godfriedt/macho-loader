
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

    // important header info
    struct mach_header_64 header;
    header.magic = *(macho);
    header.filetype = *(macho + 3);
    header.ncmds = *(macho + 4);

    macho += 8;
    int* ret = macho;
    int ls_size;
    uint32_t main_off = 0;

    // get offset of main function to begin execution
    for (int i = 0; i < header.ncmds; i++) {
        memcpy(&ls_size, macho + 1, 4);
        if (*(macho) == LC_MAIN) {
            memcpy(&main_off, macho + 2, 4);
            break;
        } else {
            macho += ls_size / 4;
        }

    }

    // __text
    macho = ret;
    memcpy(&ls_size, macho + 1, 4);
    macho += ls_size / 4;

    macho += 28;
    uint64_t sc_size;
    memcpy(&sc_size, macho, 8);

    macho += 2;
    uint32_t off;
    memcpy(&off, macho, 4);
    if (main_off == 0) {
        main_off = off;
    }

    void* mem_ptr = mmap(0, sc_size, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANON | MAP_JIT, -1, 0);
    if (mem_ptr == MAP_FAILED) {
        perror("mmap");
    }
    memcpy(mem_ptr, beginning + (off/4), sc_size);
    mprotect(mem_ptr, sc_size, PROT_READ | PROT_EXEC);

    int (*func)() = mem_ptr;
    uint32_t sub = main_off - off;
    func += sub;

    int foo = func();

    printf("back in main");

    return foo;
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

