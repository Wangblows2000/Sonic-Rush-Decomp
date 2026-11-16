#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u32 unk4;
    u32 unk8;
    u32 unkc;
} UnkTime_02042774;

extern s32 data_022c4368;
extern u16 data_022c4360;

void func_0200f0a0(UnkTime_02042774 *t);

u16 func_02042774(u16 v) {
    if (data_022c4368 == 0) {
        UnkTime_02042774 t;
        data_022c4368 = 1;
        func_0200f0a0(&t);
        data_022c4360 = (t.unk8 & 0x1f) | ((t.unk4 & 0x1f) << 5);
    } else {
        data_022c4360 = (data_022c4360 + 1) & 0x3ff;
    }
    return (v & 0x1f) | ((data_022c4360 & 0x3ff) << 5);
}
