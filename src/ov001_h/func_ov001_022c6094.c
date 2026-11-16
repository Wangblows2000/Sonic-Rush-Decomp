#include <nitro/types.h>

extern void *gTaskCurrent;
extern u8 gDisplayMain[];
extern u8 gDisplaySub[];
void func_ov001_022c66c4(void);
void func_ov001_022c6090(void);
void func_ov001_022c6160(void *arg, void (*fn)(void));

void func_ov001_022c6094(void *arg) {
    *(s16 *)(gDisplayMain + 0x48) = -16;
    *(s16 *)(gDisplaySub + 0x48) = -16;
    *(void (**)(void))((u8 *)gTaskCurrent + 8) = func_ov001_022c66c4;
    func_ov001_022c6160(arg, func_ov001_022c6090);
}
