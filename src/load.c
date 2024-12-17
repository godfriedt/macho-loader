#include "load.h"

void p_hex(const char* data, int len) {
    for (int i = 0; i < len; i++) {
        if (((i % 16) == 0) && (i != 0)) printf("\n");
        printf("%02x ", (unsigned char) *(data + i));
    }
    printf("\n");
}