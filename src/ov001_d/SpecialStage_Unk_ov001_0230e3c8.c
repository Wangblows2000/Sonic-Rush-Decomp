#include <nitro/types.h>

typedef struct UnkStruct_0230e3c8_g {
    u8 pad0[8];
    s32 unk8;
} UnkStruct_0230e3c8_g;

typedef struct UnkStruct_0230e3c8 {
    u8 pad0[0x1c];
    s32 unk1C;
    u8 pad20[0x60 - 0x20];
    u16 unk60;
    u8 pad62[0x78 - 0x62];
    void *unk78;
    u8 pad7C[0xac - 0x7c];
    s32 unkAC;
} UnkStruct_0230e3c8;

extern UnkStruct_0230e3c8_g data_ov001_0231f65c;

void *SpecialStageGimmick_Unk_ov001_02307148(void *a, s32 b, u16 c, s32 d, s32 e);
void SpecialStage_Unk_ov001_0231066c(void *c, void *w);

void SpecialStage_Unk_ov001_0230e3c8(UnkStruct_0230e3c8 *p, s32 unused, u16 ang) {
    void *w;
    s32 i;

    if (p->unk78 == NULL)
        return;
    w = SpecialStageGimmick_Unk_ov001_02307148(p->unk78, 6, ang, 0x1000, p->unk1C + 0x78000);
    if (w == NULL)
        return;
    SpecialStage_Unk_ov001_0231066c(NULL, w);
    for (i = 0; i < 4; i++) {
        w = SpecialStageGimmick_Unk_ov001_02307148(p->unk78, 6, ang + 0x38e, 0x1000, p->unk1C + 0x8c000 + (i << 12) * 0x14);
        if (w == NULL)
            return;
        SpecialStage_Unk_ov001_0231066c(NULL, w);
        w = SpecialStageGimmick_Unk_ov001_02307148(p->unk78, 6, ang - 0x38e, 0x1000, p->unk1C + 0x8c000 + (i << 12) * 0x14);
        if (w == NULL)
            return;
        SpecialStage_Unk_ov001_0231066c(NULL, w);
    }
    w = SpecialStageGimmick_Unk_ov001_02307148(p->unk78, 6, ang, 0x1000, p->unk1C + 0x8c000 + (i << 12) * 0x14);
    if (w == NULL)
        return;
    SpecialStage_Unk_ov001_0231066c(NULL, w);
    p->unkAC = data_ov001_0231f65c.unk8;
    p->unk60 |= 8;
}
