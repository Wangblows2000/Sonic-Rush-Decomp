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
} UnkStruct_02336da8_Spr;

typedef struct {
    s16 x;
    s16 y;
    UnkStruct_02336da8_Spr a;
    UnkStruct_02336da8_Spr b;
    UnkStruct_02336da8_Spr c[3];
    s32 unk270;
    u32 unk274;
} UnkStruct_02336da8_Sub;

typedef struct {
    u8 pad0[4];
    s16 unk4;
    s16 unk6;
    u8 pad8[4];
    u32 unkC;
    u32 unk10;
    u32 unk14;
    u32 unk18;
    union {
        UnkStruct_02336da8_Sub sub;
        struct {
            u8 pad[0x28C - 0x1C];
            s32 unk28C;
        } b;
    } u;
} UnkStruct_02336da8;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_02336da8_4560;

extern UnkStruct_02336da8_4560 gGameState;

void AnimatorSpriteDS_Draw(UnkStruct_02336da8_Spr *spr);
void func_02033544(UnkStruct_02336da8_Spr *spr, u16 a);
void AnimatorSpriteDS_ProcessAnimation(UnkStruct_02336da8_Spr *spr, s32 a, s32 b);

static inline void UnkSetPos_02336da8(UnkStruct_02336da8_Spr *spr, s16 x, s16 y) {
    spr->unk48 = x;
    spr->unk4A = y;
    spr->unk4C = spr->unk48;
    spr->unk4E = spr->unk4A - 0xc0;
    AnimatorSpriteDS_Draw(spr);
}

void Hud_DrawRings(UnkStruct_02336da8 *w) {
    UnkStruct_02336da8_Sub *s; s16 x; u32 i; u32 chr; UnkStruct_02336da8_Spr *spr; u32 sh; s16 y; s32 ofs; u32 chr2; s32 y2;

    s = &w->u.sub;
    x = w->unk4 + s->x;
    y = w->unk6 + s->y + 2;
    chr = w->unkC;
    chr2 = w->unk10;

    if (w->u.b.unk28C == 0 && (gGameState.unk20 & 8)) {
        chr = w->unk14;
        chr2 = w->unk18;
    }
    i = 0;
    sh = 0;
    ofs = 0;
    spr = s->c;
    for (; i < 3; i++) {
        u32 c = (u16)((s->unk274 >> sh) & 0xf);
        func_02033544(spr, c);
        spr->unk5C = chr + c * 0x80;
        spr->unk60 = chr2 + c * 0x80;
        AnimatorSpriteDS_ProcessAnimation(spr, 0, 0);
        UnkSetPos_02336da8(spr, x + 0x30 - ofs, y + 0x10);
        spr++;
        sh += 4;
        ofs += 10;
    }
    s->a.unk48 = x;
    s->a.unk4A = y;
    s->a.unk4C = x;
    y2 = y - 0xc0;
    s->a.unk4E = y2;
    AnimatorSpriteDS_Draw(&s->a);
    s->b.unk48 = x;
    s->b.unk4A = y;
    s->b.unk4C = x;
    s->b.unk4E = y2;
    AnimatorSpriteDS_Draw(&s->b);
}
