#pragma opt_loop_invariants off
#include <ov000_c/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1;
    u16 unk2;
} UnkStruct_Ev;

typedef struct {
    u8 pad[0x18];
    u32 unk18;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;
extern u8 *gPlayers[];

void func_0206c95c(void);
UnkStruct_Ev *func_0206c938(s32 a, s32 b);
u8 *func_ov000_023355d0(void);
void MapObject_OnHit_Default(UnkStruct_Coll *a, void *b);

void func_ov000_022faa74(UnkStruct_StageObj *s) {
    UnkStruct_ObjParam *prm = s->unk1E0;
    UnkStruct_Ev *o;
    u16 i;
    u32 b;

    if (!(prm->unk4 & 0x2000))
        return;
    prm->unk4 &= ~0x2000;
    if (!(data_022c4560.unk18 & 0x20))
        return;
    func_0206c95c();
    for (i = 0; i < 2; i++) {
        if (i == gPlayers[0][0x326])
            continue;
        for (;;) {
            o = func_0206c938(2, i);
            if (o == NULL)
                break;
            b = (u32)o->unk2 + (u32)func_ov000_023355d0();
            if ((u32)s->unk1E0 != b)
                continue;
            switch (o->unk0) {
            case 1:
            case 2:
                MapObject_OnHit_Default(&s->coll0, gPlayers[1] + 0x2a0);
                break;
            case 3:
                ((void (*)(UnkStruct_Coll *, void *))s->coll0.unk1C)(&s->coll0, gPlayers[1] + 0x2a0);
                break;
            case 4:
                ((void (*)(UnkStruct_Coll *, void *))s->coll1.unk1C)(&s->coll1, gPlayers[1] + 0x2a0);
                break;
            case 5:
                ((void (*)(UnkStruct_Coll *, void *))s->coll2.unk1C)(&s->coll2, gPlayers[1] + 0x2a0);
                break;
            }
        }
    }
}
