#include <nitro/types.h>

extern s32 data_022c4db0;
void func_ov001_022de240(void *p, void *q);
void func_ov001_022df40c(void *p, void *q);
void func_ov001_022de178(void *p, void *q);
void TransformDemo_ReleaseSound(void *p, void *q);

void TransformDemo_Destructor(u8 *t) {
    u8 *p = *(u8 **)(t + 0x10);
    func_ov001_022de240(p, p + 0x454);
    func_ov001_022df40c(p, p + 0xbd4);
    func_ov001_022de178(p, p + 0xea0);
    TransformDemo_ReleaseSound(p, p + 0xea8);
    data_022c4db0 = 0;
}
