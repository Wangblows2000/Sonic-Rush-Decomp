#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padC[0x1E - 0xC];
    s16 unk1E;
    s16 unk20;
    u8 pad22[0x4C - 0x22];
    s32 unk4C;
    s32 unk50;
    s32 unk54;
    s32 unk58;
    u8 pad5C[0x64 - 0x5C];
} UnkStruct_Plr;

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_Pad;

typedef struct {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_Ctl;

typedef struct {
    UnkStruct_StageObj base;
    s32 unk20C;
    s32 unk210;
} UnkStruct_02308404;

extern UnkStruct_Plr gCamera[];

void *Gimmick_GetRoomIndex(void);
UnkStruct_Ctl *EnemyRoom_Get(s32 a, void *b);
BOOL Gimmick_CheckRoomResetOnDeath(UnkStruct_Ctl *a, s32 b);
UnkStruct_Plr *Camera_GetBySwapState(void);
UnkStruct_Plr *Camera_GetOtherBySwapState(void);
void Camera_TrySwitchScreen(void);
void MapObj8F_Action_CameraLocked(UnkStruct_02308404 *s);

void MapObj8F_State_CameraLock(UnkStruct_02308404 *s) {
    UnkStruct_Ctl *o;
    UnkStruct_Plr *p;
    UnkStruct_Plr *q;
    u8 i;
    u32 f;
    u32 m;
    s32 v;

    o = EnemyRoom_Get(1, Gimmick_GetRoomIndex());
    if (o == NULL) {
        s->base.unk54 |= 4;
        return;
    }
    if (Gimmick_CheckRoomResetOnDeath(o, 0)) {
        s->base.unk1F4 &= ~0x10;
        s->base.unk54 |= 4;
        return;
    }
    for (i = 0; i < 2; i++) {
        f = o->unk20C;
        if (f & (2 << i)) {
            p = &gCamera[i];
            if (!(((UnkStruct_Pad *)s->base.unk1E0)->unk4 & 2)) {
                m = 0x10000 << i;
                if (!(f & m)) {
                    if (p->unk4 + 0x1000 >= s->unk20C) {
                        p->unk4C = s->unk20C;
                        o->unk20C |= m;
                    } else {
                        p->unk4C = p->unk4 + 0x1000;
                    }
                }
                m = 0x40000 << i;
                if (!(o->unk20C & m)) {
                    q = (gCamera[2].unk0 & 1) ? Camera_GetBySwapState() : p;
                    v = q->unk8 + 0x1000;
                    if (v < s->base.unk14) {
                        p->unk50 = v;
                    } else {
                        p->unk50 = s->unk210;
                        o->unk20C |= m;
                    }
                }
            } else {
                m = 0x100000 << i;
                if (!(f & m)) {
                    v = p->unk4 + (p->unk1E << 8) - 0x1000;
                    if (v <= s->unk20C) {
                        p->unk54 = s->unk20C;
                        o->unk20C |= m;
                    } else {
                        p->unk54 = v;
                    }
                }
                m = 0x400000 << i;
                if (!(o->unk20C & m)) {
                    q = (gCamera[2].unk0 & 1) ? Camera_GetOtherBySwapState() : p;
                    v = q->unk8 + q->unk20 * 0xc0 - 0x1000;
                    if (v <= s->base.unk14) {
                        p->unk58 = s->unk210;
                        o->unk20C |= m;
                    } else {
                        p->unk58 = v;
                    }
                }
            }
        }
    }
    if ((gCamera[2].unk0 & 1) && !(((UnkStruct_Pad *)s->base.unk1E0)->unk4 & 2))
        Camera_TrySwitchScreen();
    if (o->unk20C & 0x1000000)
        MapObj8F_Action_CameraLocked(s);
}
