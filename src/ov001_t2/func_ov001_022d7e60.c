#include <nitro/types.h>

typedef struct {
    s32 type;
    u8 pad4[4];
    u8 anim[0x34];
} UnkEnt_022d7e60;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    UnkEnt_022d7e60 ent[6];
} UnkStruct_022d7e60;

typedef struct {
    u8 pad0[0x828];
    void *res;
} UnkRes_022d7e60;

void KeyframeAnim_Init(void *p, void *res, s32 a);

#pragma thumb on
void func_ov001_022d7e60(UnkStruct_022d7e60 *p, UnkRes_022d7e60 *r) {
    s32 n;
    s32 i;
    s32 off;
    UnkEnt_022d7e60 *e;

    n = 0;
    for (i = 0; i < 4; i++) {
        if (p->ent[i].type < 0x13) {
            n++;
        }
    }
    p->unk10 = 0;
    e = &p->ent[5];
    KeyframeAnim_Init(e->anim, r->res, 6);
    e = &p->ent[4];
    e->type = 0;
    KeyframeAnim_Init(e->anim, r->res, 0xb);
    i = 0;
    e = p->ent;
    off = (4 - n) / 2;
    for (; i < 4; i++) {
        if (e->type < 0x13) {
            KeyframeAnim_Init(e->anim, r->res, i + off + 7);
        }
        e++;
    }
}
#pragma thumb reset
