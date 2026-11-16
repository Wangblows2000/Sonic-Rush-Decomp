#include <nitro/types.h>

typedef struct UnkStruct_0230b658_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_0230b658_o;

typedef struct UnkStruct_0230b658 {
    u8 pad0[0x18];
    void *unk18;
    u8 pad1C[0x2c - 0x1c];
    s32 timer;
    s32 unk30;
    s32 unk34;
    u8 pad38[0xf8 - 0x38];
    UnkStruct_0230b658_o objA;
    UnkStruct_0230b658_o objB;
} UnkStruct_0230b658;

typedef struct UnkStruct_0230b658_k {
    u8 pad0[0x10];
    u32 unk10;
} UnkStruct_0230b658_k;

extern UnkStruct_0230b658 *data_ov001_0231f5c0;
extern s16 gDisplayMain[];
extern s16 gDisplaySub[];
extern UnkStruct_0230b658_k gTouchState;
extern void *data_02250464;

void AnimatorSprite_ProcessAnimation(void *c, u32 a, u32 b);
void func_ov001_0231323c(void *w);
void func_ov001_023159b4(void);
void Sound_PlaySeqArc(u32 *p, u32 a, u32 b);
void func_ov001_0231599c(s32 v);
void func_0201c070(void **pp, s32 a);
void func_ov001_0230b164(void);

void func_ov001_0230b658(void) {
    UnkStruct_0230b658 *g = data_ov001_0231f5c0;
    UnkStruct_0230b658_o *a = &g->objA;
    UnkStruct_0230b658_o *b = &g->objB;
    s16 v;

    if (g->timer <= 0x10) {
        gDisplayMain[0x24] = 0x10 - g->timer;
        gDisplaySub[0x24] = 0x10 - g->timer;
    }
    if (g->timer > 0x5c) {
        v = g->timer - 0x5c;
        if (v > 0x10)
            v = 0x10;
        gDisplaySub[0x24] = v;
    }
    if (g->timer <= 0xa) {
        a->x = (g->timer << 8) / 10 - 0x100;
        a->y = 0x20;
        AnimatorSprite_ProcessAnimation(a, 0, 0);
    } else if (g->timer < 0x14) {
        AnimatorSprite_ProcessAnimation(a, 0, 0);
    } else if (g->timer >= 0x14) {
        b->x = 0;
        b->y = 0x20;
        AnimatorSprite_ProcessAnimation(b, 0, 0);
        g->unk30++;
    }
    if (g->timer >= 0) {
        if (g->timer > 0x5c)
            g->unk34 = 0x20;
        else
            g->unk34++;
    }
    if (g->unk18 != NULL)
        func_ov001_0231323c(g->unk18);
    func_ov001_023159b4();
    if (g->timer == 0x5c) {
        if (!(gTouchState.unk10 & 0x40))
            return;
        Sound_PlaySeqArc(0, 0x10, 0x14);
    }
    switch (g->timer) {
    case 0x28:
        func_ov001_0231599c(0xa);
        break;
    case 0x5c:
        func_ov001_0231599c(0xb);
        break;
    case 0xf0:
        func_0201c070(&data_02250464, 0x3c);
        break;
    case 0x12c:
        func_ov001_0230b164();
        break;
    }
    g->timer++;
}
