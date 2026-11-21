#include <nitro/types.h>

void __fill_mem(void *p);

void *memset(void *p) {
    __fill_mem(p);
    return p;
}
