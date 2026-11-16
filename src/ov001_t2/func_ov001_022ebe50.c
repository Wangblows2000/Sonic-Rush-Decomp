#include <nitro/types.h>

typedef struct {
    void *unk0;
    u8 pad4[0x30];
    s32 unk34;
    u8 pad38[0x10];
} UnkSpr_022ebe50;

typedef struct {
    u8 pad0[0x228];
    UnkSpr_022ebe50 spr[31];
} UnkStruct_022ebe50;

void ObjVram_Free(void *p, s32 q);
void MIi_CpuClear16(u16 data, void *dest, u32 size);

#pragma thumb on
void func_ov001_022ebe50(UnkStruct_022ebe50 *p) {
    s32 i;

    for (i = 0; i < 31; i++) {
        if (p->spr[i].unk34) {
            ObjVram_Free(p->spr[i].unk0, p->spr[i].unk34);
        }
        {
            vu16 zero = 0;
            MIi_CpuClear16(zero, &p->spr[i], sizeof(UnkSpr_022ebe50));
        }
    }
}
#pragma thumb reset
