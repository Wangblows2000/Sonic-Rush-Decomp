#include <nitro/types.h>

typedef struct UnkStruct_023119ac_o {
    u8 pad0[0x10c];
    u8 unk10C;
} UnkStruct_023119ac_o;

typedef struct UnkStruct_023119ac {
    u8 pad0[0x48];
    s32 unk48;
    u8 pad4C[0x54 - 0x4c];
    s32 unk54;
    u8 pad58[0xac - 0x58];
    UnkStruct_023119ac_o *unkAC;
    u8 padB0[0xe4 - 0xb0];
    s32 x;
    s32 y;
    s32 z;
    u8 padF0[0x120 - 0xf0];
    s32 unk120;
    u8 pad124[0x148 - 0x124];
    s32 rings;
} UnkStruct_023119ac;

BOOL func_ov001_02312138(UnkStruct_023119ac *p);
void func_ov001_023122d8(UnkStruct_023119ac *p, s32 state);
void func_02036754(s32 a, s32 b, s32 c);
void func_ov001_023070fc(s32 type, s32 x, s32 y, s32 z);

void func_ov001_023119ac(UnkStruct_023119ac *p) {
    s32 i;

    if (func_ov001_02312138(p)) {
        p->unk48 = 0;
        p->unk54 = 0;
    }
    if (p->unkAC->unk10C & 2) {
        func_ov001_023122d8(p, 0x11);
        return;
    }
    if (p->unk120 == 0 && p->rings > 0) {
        func_02036754(0, 0x10, 4);
        for (i = 0; i < p->rings; i++) {
            func_ov001_023070fc(4, p->x, p->y, p->z);
            if (i == 9)
                break;
        }
        p->rings -= 10;
        if (p->rings < 0)
            p->rings = 0;
    }
    p->unk120 = 1;
}
