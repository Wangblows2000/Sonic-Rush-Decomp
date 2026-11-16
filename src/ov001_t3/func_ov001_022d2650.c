#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u16 unk18;
    u16 unk1a;
} UnkRes_022d2650;

typedef struct {
    u32 flags;
    u8 pad4[8];
    s32 unkc;
    u8 pad10[4];
    s32 unk14;
    u32 unk18;
    s32 unk1c;
    u8 pad20[0x20];
} UnkObj_022d2650;

typedef struct {
    u8 pad0[0x18];
    UnkRes_022d2650 *res[15];
    UnkObj_022d2650 objs[15];
} UnkStruct_022d2650;

extern u8 data_ov001_023160f8[];
extern u32 data_ov001_02316118[];

void Background_Init(UnkObj_022d2650 *o, UnkRes_022d2650 *r, s32 a, u32 b, u8 c, u16 d, u16 e);

#pragma thumb on
void func_ov001_022d2650(UnkStruct_022d2650 *p) {
    s32 i;
    UnkRes_022d2650 *r;
    UnkObj_022d2650 *o;

    for (i = 0; i < 15; i++) {
        r = p->res[i];
        if (r != NULL) {
            Background_Init(&p->objs[i], r, 0x38, data_ov001_02316118[i], data_ov001_023160f8[i], r->unk18, r->unk1a);
        }
    }
    p->objs[0].unk1c += 0x2000;
    p->objs[1].unk1c += 0x2000;
    for (i = 4; i < 10; i++) {
        o = &p->objs[i];
        o->unk14 = 0;
        if (o->unkc == 0) {
            o->unk18 = 0x5000000;
        } else {
            o->unk18 = 0x5000400;
        }
        o->flags &= ~0x20;
        o->flags |= 4;
    }
}
#pragma thumb reset
