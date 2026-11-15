#include <nitro/types.h>

void func_020371c8(void *p);
void func_ov001_022d2e14(void *p);
void func_ov001_022d2234(void *p, void (*f)(void));

#pragma thumb on
void func_ov001_022d1c9c(u8 *p) {
    if (*(void **)(p + 8) != NULL) {
        func_020371c8(*(void **)(p + 8));
        *(void **)(p + 8) = NULL;
    }
    func_ov001_022d2e14(p);
    func_ov001_022d2234(p, NULL);
}
#pragma thumb reset
