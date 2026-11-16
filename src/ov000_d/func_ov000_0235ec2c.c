#include <nitro/types.h>

typedef struct {
    void *unk0;
    u8 pad4[0x34 - 4];
    void *unk34;
    u8 pad38[0x48 - 0x38];
} UnkSpr_0235ec2c;

typedef struct {
    u8 pad0[0x24];
    UnkSpr_0235ec2c spr[0x12];
} UnkStruct_0235ec2c;

void func_02039bb0(void *a, void *b);
void MIi_CpuClear16(u16 data, void *dest, u32 size);

#pragma thumb on

void func_ov000_0235ec2c(UnkStruct_0235ec2c *w) {
    s32 i;
    UnkSpr_0235ec2c *spr;

    for (i = 0, spr = w->spr; i < 0x12; i++) {
        vu16 zero;
        if (spr->unk34 != NULL) {
            func_02039bb0(spr->unk0, spr->unk34);
        }
        zero = 0;
        MIi_CpuClear16(zero, spr, sizeof(UnkSpr_0235ec2c));
        spr++;
    }
}

#pragma thumb reset
