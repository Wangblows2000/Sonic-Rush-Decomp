#include <nitro/types.h>

typedef struct {
    s32 type;
    u8 pad4[4];
    u8 anim[0x34];
} UnkEnt_022d7dbc;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    UnkEnt_022d7dbc ent[6];
} UnkStruct_022d7dbc;

typedef struct {
    u8 pad0[0x828];
    void *res;
} UnkRes_022d7dbc;

void KeyframeAnim_Init(void *p, void *res, s32 a);
void KeyframeAnim_PlayReverse(void *p);

#pragma thumb on
void func_ov001_022d7dbc(UnkStruct_022d7dbc *p, UnkRes_022d7dbc *r) {
    s32 n;
    s32 i;
    s32 off;
    UnkEnt_022d7dbc *e;

    n = 0;
    for (i = 0; i < 4; i++) {
        if (p->ent[i].type < 0x13) {
            n++;
        }
    }
    p->unk10 = 1;
    e = &p->ent[5];
    KeyframeAnim_Init(e->anim, r->res, 0);
    KeyframeAnim_PlayReverse(e->anim);
    e = &p->ent[4];
    e->type = 0;
    KeyframeAnim_Init(e->anim, r->res, 5);
    KeyframeAnim_PlayReverse(e->anim);
    i = 0;
    e = p->ent;
    off = (4 - n) / 2;
    for (; i < 4; i++) {
        if (e->type < 0x13) {
            KeyframeAnim_Init(e->anim, r->res, i + 1 + off);
            KeyframeAnim_PlayReverse(e->anim);
        }
        e++;
    }
}
#pragma thumb reset
