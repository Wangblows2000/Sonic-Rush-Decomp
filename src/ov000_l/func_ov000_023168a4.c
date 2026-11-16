#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x13];
    s32 unk14;
    u32 unk18;
} UnkPlayer_023168a4;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkColl_023168a4;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;

void func_ov000_02316a58(UnkStruct_StageObj *o);
void func_ov000_02355afc(UnkPlayer_023168a4 *p);
void func_ov000_02355b20(UnkPlayer_023168a4 *p, s32 vx, s32 vy);
void func_ov000_02355da0(UnkPlayer_023168a4 *p, UnkStruct_StageObj *o);

void func_ov000_023168a4(UnkColl_023168a4 *a, UnkColl_023168a4 *b) {
    UnkStruct_StageObj *o = a->unk18;
    UnkPlayer_023168a4 *p = b->unk18;
    UnkStruct_ObjParam *prm;
    u16 id;
    s16 vx;
    s16 vy;

    if (o == NULL)
        return;
    if (p == NULL && p->unk0 != 1)
        return;
    if ((p->unk18 & ~0x3ff) != (o->unk18 & ~0x3ff))
        return;
    func_ov000_02316a58(o);
    prm = o->unk1E0;
    id = prm->unk2;
    vx = -0x780;
    vx -= prm->unk6 * 0x180;
    vy = vx;
    if (id <= 0x44 || id > 0x4c)
        vx = 0;
    else if (id <= 0x46)
        vy = 0;
    if (id >= 0x47 && id <= 0x4c) {
        vx = (vx * 0xb5) >> 8;
        vy = (vy * 0xb5) >> 8;
    }
    if (o->unk5C & 2)
        vy = -vy;
    if (o->unk5C & 1)
        vx = -vx;
    if ((id == 0x45 || id == 0x46) && !(prm->unk4 & 2))
        p->unk14 = o->unk14 + 0x200;
    if (data_022c4560.unk1C == 3)
        func_ov000_02355afc(p);
    else
        func_ov000_02355b20(p, vx, vy);
    func_ov000_02355da0(p, o);
}
