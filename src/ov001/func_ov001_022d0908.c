#include <nitro/types.h>

void func_ov001_022d0220(void *p);
void func_ov001_022d1404(void *p, void (*f)(void));
void func_ov001_022d07cc(void);

#pragma thumb on
void func_ov001_022d0908(u8 *p) {
    func_ov001_022d0220(p);
    if (*(s32 *)(p + 0xbdc) > 0x78) {
        func_ov001_022d1404(p, func_ov001_022d07cc);
    }
}
#pragma thumb reset
