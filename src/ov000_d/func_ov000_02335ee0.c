#include <nitro/types.h>

typedef struct {
    u8 pad0[0x48];
    s16 unk48;
    s16 unk4A;
    s16 unk4C;
    s16 unk4E;
    u8 pad50[0x5C - 0x50];
    u32 unk5C;
    u32 unk60;
    u8 pad64[0x7C - 0x64];
} UnkStruct_02335ee0_Spr;

typedef struct {
    s16 x;
    s16 y;
    u8 pad4[4];
    UnkStruct_02335ee0_Spr spr[3];
    u32 unk17C;
    u32 unk180;
} UnkStruct_02335ee0_Sub;

typedef struct {
    u32 unk0;
    s16 unk4;
    s16 unk6;
    u8 pad8[4];
    u32 unkC;
    u32 unk10;
    u8 pad14[0x924 - 0x14];
    UnkStruct_02335ee0_Sub sub;
} UnkStruct_02335ee0;

void AnimatorSpriteDS_Draw(UnkStruct_02335ee0_Spr *spr);
void AnimatorSprite_Unk_02033544(UnkStruct_02335ee0_Spr *spr, u16 a);
void AnimatorSpriteDS_ProcessAnimation(UnkStruct_02335ee0_Spr *spr, s32 a, s32 b);

static inline void UnkSetPos_02335ee0(UnkStruct_02335ee0_Spr *spr, s16 x, s16 y) {
    spr->unk48 = x;
    spr->unk4A = y;
    spr->unk4C = spr->unk48;
    spr->unk4E = spr->unk4A - 0xc0;
    AnimatorSpriteDS_Draw(spr);
}

void func_ov000_02335ee0(UnkStruct_02335ee0 *w) {
    UnkStruct_02335ee0_Sub *s = &w->sub;
    s16 x = s->x + w->unk4 + 4;
    s16 y = s->y + w->unk6 + 0xaa;
    UnkStruct_02335ee0_Spr *spr;
    u32 cur;

    spr = &s->spr[0];
    UnkSetPos_02335ee0(spr, x, y);
    if (w->unk0 & 0x10) {
        return;
    }
    spr = &s->spr[1];
    UnkSetPos_02335ee0(spr, x + 0x18, y + 0x12);
    cur = s->unk17C & 0xf;
    spr = &s->spr[2];
    if (cur != (s->unk180 & 0xf)) {
        u32 c = (u16)cur;
        AnimatorSprite_Unk_02033544(spr, c);
        spr->unk5C = w->unkC + c * 0x80;
        spr->unk60 = w->unk10 + c * 0x80;
        AnimatorSpriteDS_ProcessAnimation(spr, 0, 0);
    }
    UnkSetPos_02335ee0(spr, x + 0x22, y + 0x12);
}
