#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x34];
    u32 unk34;
    u16 unk38;
    u8 pad3A[0x64 - 0x3A];
} UnkStruct_Plr;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x212 - 0x20C];
    u16 unk212;
    s8 unk214[2];
} UnkStruct_02309ca4;

extern UnkStruct_StageObj *gPlayers[];
extern UnkStruct_Plr gCamera[];

u32 Camera_CheckPosInViewLatched(s32 x, s32 y);

void MapObj70_State_Unk_ov000_02309ca4(UnkStruct_02309ca4 *w) {
    u8 i;
    UnkStruct_Plr *p;
    u16 f;

    for (i = 0; i < 2; i = (u8)(i + 1)) {
        if (w->unk214[i] < 0)
            continue;
        if (!((1 << i) & Camera_CheckPosInViewLatched(w->base.unk10, w->base.unk14)))
            continue;
        p = &gCamera[i];
        f = w->base.unk1E0->unk4;
        if (f & 3) {
            if (gPlayers[w->unk214[i]]->unk10 < w->base.unk10) {
                if (f & 1) {
                    if (!(f & 0x20))
                        p->unk34 &= ~0x10;
                } else {
                    p->unk34 |= 0x10;
                    p->unk38 = w->unk212;
                }
            } else {
                if (f & 1) {
                    p->unk34 |= 0x10;
                    p->unk38 = w->unk212;
                } else if (!(f & 0x20)) {
                    p->unk34 &= ~0x10;
                }
            }
        } else if (f & 0xc) {
            if (gPlayers[w->unk214[i]]->unk14 < w->base.unk14) {
                if (f & 4) {
                    if (!(f & 0x20))
                        p->unk34 &= ~0x10;
                } else {
                    p->unk34 |= 0x10;
                    p->unk38 = w->unk212;
                }
            } else {
                if (f & 8) {
                    p->unk34 |= 0x10;
                    p->unk38 = w->unk212;
                } else if (!(f & 0x20)) {
                    p->unk34 &= ~0x10;
                }
            }
        }
    }
}
