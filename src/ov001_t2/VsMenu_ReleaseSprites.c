#include <nitro/types.h>

typedef struct {
    void *unk0;
    u8 pad4[0x30];
    s32 unk34;
    u8 pad38[0x10];
} UnkSpr_022dd728;

typedef struct {
    u8 pad0[0x210];
    UnkSpr_022dd728 spr[31];
} UnkStruct_022dd728;

void ObjVram_Free(void *p, s32 q);
void MIi_CpuClear16(u16 data, void *dest, u32 size);

#pragma thumb on
void VsMenu_ReleaseSprites(UnkStruct_022dd728 *p) {
    s32 i;

    for (i = 0; i < 31; i++) {
        if (p->spr[i].unk34) {
            ObjVram_Free(p->spr[i].unk0, p->spr[i].unk34);
        }
        {
            vu16 zero = 0;
            MIi_CpuClear16(zero, &p->spr[i], sizeof(UnkSpr_022dd728));
        }
    }
}
#pragma thumb reset
