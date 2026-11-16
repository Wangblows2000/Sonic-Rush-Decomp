#include <nitro/types.h>

typedef struct UnkStruct_022e85f8 {
    u8 pad0[0x3d0];
    s32 unk3D0;
    u8 pad3D4[0x440 - 0x3d4];
    void (*unk440)(struct UnkStruct_022e85f8 *p);
} UnkStruct_022e85f8;

typedef struct {
    u8 pad0[4];
    s32 unk4;
} UnkStruct_02369ac0;

extern UnkStruct_02369ac0 data_ov000_02369ac0;

void func_ov000_022e8478(UnkStruct_022e85f8 *p);
void func_ov000_022e7a84(UnkStruct_022e85f8 *p);

void func_ov000_022e85f8(UnkStruct_022e85f8 *p) {
    data_ov000_02369ac0.unk4 = 0x2c;
    if (p->unk3D0 == 0)
        p->unk440 = func_ov000_022e8478;
    else
        p->unk440 = func_ov000_022e7a84;
    p->unk440(p);
}
