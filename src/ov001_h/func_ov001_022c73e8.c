#include <nitro/types.h>

extern void *gTaskCurrent;
extern u8 gDisplayMain[];
extern u8 gDisplaySub[];
void func_ov001_022c8458(void);
void func_ov001_022c7a34(void *arg, s32 a1);

void func_ov001_022c73e8(void *arg) {
    *(s16 *)(gDisplayMain + 0x48) = 16;
    *(s16 *)(gDisplaySub + 0x48) = 16;
    *(void (**)(void))((u8 *)gTaskCurrent + 8) = func_ov001_022c8458;
    func_ov001_022c7a34(arg, 0);
}
