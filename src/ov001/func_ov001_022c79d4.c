#include <nitro/types.h>

extern u16 gDisplayMain[];
void Ending_SetSubState(void *p, void (*f)(void));
void func_ov001_022c7940(void);

void func_ov001_022c79d4(void *p) {
    gDisplayMain[0x24] = 0x10;
    Ending_SetSubState(p, func_ov001_022c7940);
}
