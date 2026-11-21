#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x14];
    u16 unk44;
    u16 unk46;
} UnkSpr_022d6864;

typedef struct { u16 unk0; u16 unk2; u16 unk4; u8 pad6[0xe]; } UnkEnt_022d6864;
typedef struct { u8 pad0[0x1c]; UnkEnt_022d6864 ent[1]; } UnkTbl_022d6864;

typedef struct {
    u8 pad0[0x18];
    s32 offset;
} UnkRes_022d6864;

void *ObjVram_Alloc(u32 a, u32 b);
void AnimatorSprite_Init(UnkSpr_022d6864 *s, UnkRes_022d6864 *res, u32 idx, u32 c, u32 d, u32 e, void *f, u32 g, void *h);
void AnimatorSprite_SetAnimation(UnkSpr_022d6864 *s, u16 anim);

#pragma thumb on
void TitleDemo_InitSubSprite(UnkSpr_022d6864 *s, UnkRes_022d6864 *res, u16 idx, s32 mode, u16 a, u16 b) {
    UnkTbl_022d6864 *t = (UnkTbl_022d6864 *)((u8 *)res + res->offset);
    AnimatorSprite_Init(s, res, idx, 0, 1, 0, ObjVram_Alloc(1, t->ent[idx].unk4), 4, NULL);
    s->flags &= ~0x60000;
    s->flags |= mode << 17;
    s->unk44 = a;
    s->unk46 = b;
    AnimatorSprite_SetAnimation(s, idx);
}
#pragma thumb reset
