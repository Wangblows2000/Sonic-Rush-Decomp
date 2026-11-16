#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 sel;
} UnkStruct_022da614;

void func_ov001_022d9e50(UnkStruct_022da614 *p, s32 x, s32 y);
void func_ov001_022d9e2c(UnkStruct_022da614 *p, s32 x, s32 y);
void func_ov001_022d9b3c(UnkStruct_022da614 *p, s32 i, s32 x, s32 y);
void func_ov001_022d9b0c(UnkStruct_022da614 *p, s32 x, s32 y);
s32 func_02050f60(void);
s32 func_02050ee4(s32 i);
void func_ov001_022d9ab4(UnkStruct_022da614 *p, s32 x, s32 y, s32 a, s32 b);

#pragma thumb on
void func_ov001_022da614(UnkStruct_022da614 *p) {
    s32 y;
    s32 i;
    s32 max;

    func_ov001_022d9e50(p, 0x38, 0x98);
    func_ov001_022d9e2c(p, 0x10, 0x48);
    y = 0x20;
    for (i = 0; i < 8; i++) {
        func_ov001_022d9b3c(p, i, 0x38, y);
        if (p->sel == i) {
            func_ov001_022d9b0c(p, 0x38, y);
        }
        y += 0x11;
    }
    max = 0;
    for (i = 0; i < func_02050f60(); i++) {
        if (max < func_02050ee4(i)) {
            max = func_02050ee4(i);
        }
    }
    func_ov001_022d9ab4(p, 0xb0, 8, 0, max);
}
#pragma thumb reset
