#include <nitro/types.h>

void func_ov001_022d0220(void *p);
void StaffRoll_SetPictureState(void *p, void (*f)(void));
void func_ov001_022d0a5c(void);

#pragma thumb on
void func_ov001_022d0af0(u8 *p) {
    func_ov001_022d0220(p);
    if (*(u32 *)(p + 0xc58) & 2) {
        StaffRoll_SetPictureState(p, func_ov001_022d0a5c);
    }
}
#pragma thumb reset
