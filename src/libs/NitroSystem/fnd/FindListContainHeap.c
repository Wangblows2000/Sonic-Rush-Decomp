#include <nitro/types.h>

extern u8 data_02084d2c[];
u8 *FindContainHeap(void *list, void *p);

void *FindListContainHeap(void *p) {
    u8 *r = data_02084d2c;
    u8 *x = FindContainHeap(r, p);
    if (x != NULL) r = x + 0xC;
    return r;
}
