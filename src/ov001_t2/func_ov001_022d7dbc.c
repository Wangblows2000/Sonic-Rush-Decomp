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

void func_02040ef0(void *p, void *res, s32 a);
void func_02040e80(void *p);

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
    func_02040ef0(e->anim, r->res, 0);
    func_02040e80(e->anim);
    e = &p->ent[4];
    e->type = 0;
    func_02040ef0(e->anim, r->res, 5);
    func_02040e80(e->anim);
    i = 0;
    e = p->ent;
    off = (4 - n) / 2;
    for (; i < 4; i++) {
        if (e->type < 0x13) {
            func_02040ef0(e->anim, r->res, i + 1 + off);
            func_02040e80(e->anim);
        }
        e++;
    }
}
#pragma thumb reset
