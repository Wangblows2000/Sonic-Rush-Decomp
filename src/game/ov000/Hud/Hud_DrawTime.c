#pragma opt_dead_assignments off
#include <nitro/types.h>
#include "main_a2/dual.h"

typedef struct {
    s16 x;
    s16 y;
    UnkDual d4;
    UnkDual d80;
    UnkDual dfc[2];
    UnkDual d1f4[2];
    UnkDual d2ec;
    UnkDual d368;
    u32 time;
    u32 digits;
} UnkSub_023369bc;

typedef struct {
    u32 flags;
    s16 unk4;
    s16 unk6;
    u8 pad8[4];
    u32 vram[4];
    u8 pad1c[0x27c];
    UnkSub_023369bc sub;
} UnkWork_023369bc;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 gGameState;

void AnimatorSpriteDS_ProcessAnimation(UnkDual *d, s32 a, s32 b);
void AnimatorSpriteDS_SetAnimation(UnkDual *d, u16 idx);
void AnimatorSpriteDS_Draw(UnkDual *d);

void Hud_DrawTime(UnkWork_023369bc *work) {
    UnkSub_023369bc *sub;
    s16 x;
    UnkDual *d;
    u16 i;
    u32 vb;
    u32 va;
    s16 off;
    s32 px;
    s32 px2;
    s16 y;
    s32 pq;
    s32 dig;

    x = work->unk4 + work->sub.x + 0xa8;
    sub = &work->sub;
    vb = work->vram[0];
    va = work->vram[1];
    y = work->unk6 + work->sub.y + 0x12;
    off = 0;
    if (work->sub.time >= 0x7e90 && (gGameState.unk20 & 8)) {
        vb = work->vram[2];
        va = work->vram[3];
    }

    px = x;
    for (i = 0; i < 2; i++) {
        dig = (u16)((sub->digits >> (u16)(i * 4)) & 0xf);
        off -= 10;
        d = &sub->d1f4[i];
        px -= 10;
        AnimatorSpriteDS_SetAnimation(d, dig);
        d->unk5c[0] = vb + dig * 0x80;
        d->unk5c[1] = va + dig * 0x80;
        AnimatorSpriteDS_ProcessAnimation(d, 0, 0);
        d->ab[0].a = px;
        d->ab[0].b = y;
        d->ab[1].a = d->ab[0].a;
        d->ab[1].b = d->ab[0].b - 0xc0;
        AnimatorSpriteDS_Draw(d);
    }
    d = &sub->d2ec;
    d->ab[0].a = x + (s16)(off - 8);
    d->ab[0].b = y;
    d->ab[1].a = d->ab[0].a;
    d->ab[1].b = d->ab[0].b - 0xc0;
    AnimatorSpriteDS_Draw(d);

    off -= 6;
    pq = x + off;
    for (i = 0; i < 2; i++) {
        s32 dig2 = (u16)((sub->digits >> (u16)((i + 2) * 4)) & 0xf);
        off -= 10;
        d = &sub->dfc[i];
        pq -= 10;
        AnimatorSpriteDS_SetAnimation(d, dig2);
        d->unk5c[0] = vb + dig2 * 0x80;
        d->unk5c[1] = va + dig2 * 0x80;
        AnimatorSpriteDS_ProcessAnimation(d, 0, 0);
        d->ab[0].a = pq;
        d->ab[0].b = y;
        d->ab[1].a = d->ab[0].a;
        d->ab[1].b = d->ab[0].b - 0xc0;
        AnimatorSpriteDS_Draw(d);
    }
    d = &sub->d368;
    d->ab[0].a = x + (s16)(off - 8);
    d->ab[0].b = y;
    d->ab[1].a = d->ab[0].a;
    d->ab[1].b = d->ab[0].b - 0xc0;
    AnimatorSpriteDS_Draw(d);

    dig = (u16)((sub->digits >> 16) & 0xf);
    px2 = x + (s16)(off - 6);
    d = &sub->d80;
    AnimatorSpriteDS_SetAnimation(d, dig);
    d->unk5c[0] = vb + dig * 0x80;
    d->unk5c[1] = va + dig * 0x80;
    AnimatorSpriteDS_ProcessAnimation(d, 0, 0);
    d->ab[0].a = px2 - 10;
    d->ab[0].b = y;
    d->ab[1].a = d->ab[0].a;
    d->ab[1].b = d->ab[0].b - 0xc0;
    AnimatorSpriteDS_Draw(d);

    d = &sub->d4;
    d->ab[0].a = x + (s16)((s16)(off - 16) - 14);
    d->ab[0].b = y;
    d->ab[1].a = d->ab[0].a;
    d->ab[1].b = d->ab[0].b - 0xc0;
    AnimatorSpriteDS_Draw(d);
}
