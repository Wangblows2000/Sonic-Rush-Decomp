#include <game/Stage/stageobj.h>
typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x23C - 0x20C];
    s8 unk23C[4];
} UnkStruct_02302e2c;
extern UnkStruct_StageObj *gPlayers[];


BOOL MapObj75_IsPlayerInArea(UnkStruct_02302e2c *s, s32 i) {
    BOOL res;
    BOOL in;
    UnkStruct_StageObj *p;
    p = gPlayers[s->unk23C[i]];
    res = FALSE;
    in = FALSE;
    {
        s32 lo; s32 v; BOOL r;
        v = p->unk10;
        r = FALSE;
        lo = s->base.unk10 + (s->base.unk1E0->unk6 << 8);
        if (lo <= v && lo + (s->base.unk1E0->unk8 << 8) >= v)
            r = TRUE;
        if (r) {
            BOOL r2;
            v = p->unk14;
            r2 = FALSE;
            lo = s->base.unk14 + (s->base.unk1E0->unk7 << 8);
            if (lo <= v && lo + (s->base.unk1E0->unk9 << 8) >= v)
                r2 = TRUE;
            if (r2)
                in = TRUE;
        }
    }
    if (in) {
        BOOL a = TRUE;
        BOOL b = FALSE;
        if ((p->unk54 & 1) && (s->base.unk1E0->unk4 & 0x20))
            b = a;
        if (!b && ((p->unk54 & 1) || !(s->base.unk1E0->unk4 & 0x10)))
            a = FALSE;
        if (a)
            res = TRUE;
    }
    return res;
}
