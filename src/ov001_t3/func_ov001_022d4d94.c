#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2c];
    u32 flags;
    u8 pad30[0x46 - 0x30];
    u16 unk46;
} UnkSpr_022d4d94;

typedef struct {
    u8 pad0[0x18];
    s32 offset;
} UnkRes_022d4d94;

typedef struct {
    u8 pad0[4];
    UnkSpr_022d4d94 sprB;
    UnkSpr_022d4d94 sprA;
    s32 unk94;
    s32 unk98;
    u8 pad9c[0x15c - 0x9c];
    UnkRes_022d4d94 *resA;
    UnkRes_022d4d94 *resB;
} UnkStruct_022d4d94;

u32 ObjVram_Alloc(s32 a, u16 b);
void AnimatorSprite_Init(UnkSpr_022d4d94 *s, UnkRes_022d4d94 *r, s32 a, s32 b, s32 c, s32 d, u32 e, s32 f, u32 g);

#pragma thumb on
void func_ov001_022d4d94(UnkStruct_022d4d94 *p) {
    UnkRes_022d4d94 *r;
    UnkSpr_022d4d94 *s;

    r = p->resA;
    s = &p->sprA;
    AnimatorSprite_Init(s, r, 0, 0, 1, 0, ObjVram_Alloc(1, *(u16 *)((u8 *)r + r->offset + 0xa)), 0, 0x5000600);
    s->flags &= ~0x60000;
    s->flags |= 0x40000;
    s->unk46 = 8;
    r = p->resB;
    s = &p->sprB;
    AnimatorSprite_Init(s, r, 0, 0, 1, 0, ObjVram_Alloc(1, *(u16 *)((u8 *)r + r->offset + 0xa)), 0, 0x5000600);
    s->flags &= ~0x60000;
    s->flags |= 0x40000;
    s->unk46 = 4;
    p->unk94 = -1;
    p->unk98 = -1;
}
#pragma thumb reset
