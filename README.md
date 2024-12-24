# macho-loader
work in progress macho loader written in C

- https://blog.xpnsec.com/building-a-mach-o-memory-loader-part-1/
- https://github.com/aidansteele/osx-abi-macho-file-format-reference
- https://en.wikipedia.org/wiki/Mach-O

### usage:
```
gcc main.c load.c
./a.out <macho_file>
```

### next steps

- import symbols
