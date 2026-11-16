/* paramclone of PlayerSelectMenu_ReleaseSprites (src/ov000_d/PlayerSelectMenu_ReleaseSprites.c); immediates and relocations remapped. */
#include <nitro/types.h>

typedef struct {
    void *unk0;
    u8 pad4[0x34 - 4];
    void *unk34;
    u8 pad38[0x48 - 0x38];
} UnkSpr_022d66f8;

typedef struct {
    u8 pad0[0x29c];
    UnkSpr_022d66f8 spr[0x4];
} UnkStruct_022d66f8;

void ObjVram_Free(void *a, void *b);
void MIi_CpuClear16(u16 data, void *dest, u32 size);

#pragma thumb on

void TitleDemo_ReleaseSprites(UnkStruct_022d66f8 *w) {
    s32 i;
    UnkSpr_022d66f8 *spr;

    for (i = 0, spr = w->spr; i < 0x4; i++) {
        vu16 zero;
        if (spr->unk34 != NULL) {
            ObjVram_Free(spr->unk0, spr->unk34);
        }
        zero = 0;
        MIi_CpuClear16(zero, spr, sizeof(UnkSpr_022d66f8));
        spr++;
    }
}

#pragma thumb reset
