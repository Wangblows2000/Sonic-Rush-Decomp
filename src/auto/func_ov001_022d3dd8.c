/* paramclone of func_ov000_0235ec2c (src/ov000_d/func_ov000_0235ec2c.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct {
    void *unk0;
    u8 pad4[0x34 - 4];
    void *unk34;
    u8 pad38[0x48 - 0x38];
} UnkSpr_022d3dd8;

typedef struct {
    u8 pad0[0xc];
    UnkSpr_022d3dd8 spr[0x6];
} UnkStruct_022d3dd8;

void func_02039bb0(void *a, void *b);
void MIi_CpuClear16(u16 data, void *dest, u32 size);

#pragma thumb on

void func_ov001_022d3dd8(UnkStruct_022d3dd8 *w) {
    s32 i;
    UnkSpr_022d3dd8 *spr;

    for (i = 0, spr = w->spr; i < 0x6; i++) {
        vu16 zero;
        if (spr->unk34 != NULL) {
            func_02039bb0(spr->unk0, spr->unk34);
        }
        zero = 0;
        MIi_CpuClear16(zero, spr, sizeof(UnkSpr_022d3dd8));
        spr++;
    }
}

#pragma thumb reset
