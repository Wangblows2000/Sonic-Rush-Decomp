#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x18];
    void *unk18;
} UnkStruct_Hit;

typedef struct {
    s32 x;
    s32 y;
    s32 z;
    u8 pad[0x144 - 0xC];
} UnkStruct_Seg;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x218 - 0x20C];
    UnkStruct_Seg unk218[2];
} UnkStruct_022f6cb0;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad[0x3b2 - 0x20c];
    s16 unk3B2;
} UnkStruct_Player;

typedef struct {
    u8 data[0x18];
} UnkStruct_Line;

void func_02038458(UnkStruct_Vec *a, UnkStruct_Vec *b, UnkStruct_Line *c);
BOOL func_02038374(UnkStruct_Line *l, UnkStruct_Vec *a, UnkStruct_Vec *b, s32 r, UnkStruct_Vec *p);
void Player_OnHurt(UnkStruct_Hit *b, UnkStruct_Hit *a);

void BossFPartDE_OnHit(UnkStruct_Hit *a, UnkStruct_Hit *b) {
    UnkStruct_022f6cb0 *w;
    UnkStruct_Player *p;
    UnkStruct_Line line;
    UnkStruct_Vec pp;
    UnkStruct_Vec wp;
    UnkStruct_Vec ep;
    u32 i;

    p = b->unk18;
    w = a->unk18;
    if (p->unk3B2 > 0)
        return;
    for (i = 0; i < 2; i++) {
        pp.x = p->base.unk10 << 4;
        pp.y = -p->base.unk14 << 4;
        pp.z = p->base.unk18 << 4;
        wp.x = w->base.unk10 << 4;
        wp.y = -w->base.unk14 << 4;
        wp.z = w->base.unk18 << 4;
        ep.x = w->unk218[i].x << 4;
        ep.y = -w->unk218[i].y << 4;
        ep.z = w->unk218[i].z << 4;
        func_02038458(&wp, &ep, &line);
        if (func_02038374(&line, &wp, &ep, 0xc000, &pp)) {
            Player_OnHurt(b, a);
            return;
        }
    }
}
