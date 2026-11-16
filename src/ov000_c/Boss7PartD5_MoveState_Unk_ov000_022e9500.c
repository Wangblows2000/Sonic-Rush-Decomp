#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x3D0 - 0x20C];
    s32 unk3D0;
    u8 pad3D4[0x3D8 - 0x3D4];
    u16 unk3D8;
    u8 pad3DA[0x3DC - 0x3DA];
    u16 unk3DC;
    u8 pad3DE[0x430 - 0x3DE];
    s16 unk430;
    u8 pad432[0x440 - 0x432];
    void *unk440;
    u8 pad444[0x464 - 0x444];
    u16 unk464;
    u16 unk466;
} UnkStruct_022e9500;

extern s16 data_ov000_02368b80[][0x18];
extern s16 gPlayerPhysicsTable[][0x18];

void Boss7PartD5_Unk_ov000_022e99e0(UnkStruct_022e9500 *w);
void Boss7PartD5_Unk_ov000_022e9990(UnkStruct_022e9500 *w, s32 v);
void Boss7PartD5_Unk_ov000_022e85f8(UnkStruct_022e9500 *w);
BOOL Boss7PartD5_Unk_ov000_022e9b28(UnkStruct_022e9500 *w);
BOOL Boss7PartD5_Unk_ov000_022e9b04(UnkStruct_022e9500 *w);
void Boss7PartD5_MoveState_Unk_ov000_022e9488(void);
void Boss7PartD5_MoveState_Unk_ov000_022e98d8(void);

void Boss7PartD5_MoveState_Unk_ov000_022e9500(UnkStruct_022e9500 *w) {
    u16 f = w->unk3D8;

    if (!(f & 0x30)) {
        s16 d = data_ov000_02368b80[w->unk3D0][0];
        d = d + (d >> 1);
        if (w->base.unk2E < 0) {
            if (d < -w->base.unk2E)
                w->base.unk2E += d;
            else
                w->base.unk2E = 0;
        } else if (w->base.unk2E > 0) {
            if (d < w->base.unk2E)
                w->base.unk2E -= d;
            else
                w->base.unk2E = 0;
        }
    } else {
        s16 a = gPlayerPhysicsTable[w->unk3D0][0];
        s32 lim = w->unk430;
        if (f & 0x20) {
            if (w->unk464 == 0 && !(w->base.unk5C & 1)) {
                w->unk464 = 6;
                w->unk466 = 0;
                w->base.unk5C |= 1;
            }
            a = -gPlayerPhysicsTable[w->unk3D0][0];
        } else if (f & 0x10) {
            if (w->unk464 == 0 && (w->base.unk5C & 1)) {
                w->unk464 = 6;
                w->unk466 = 0;
                w->base.unk5C &= ~1;
            }
            a = gPlayerPhysicsTable[w->unk3D0][0];
        }
        w->base.unk2E += a;
        w->base.unk2E = w->base.unk2E < (s16)-lim ? (s16)-lim : (w->base.unk2E > (s16)lim ? (s16)lim : w->base.unk2E);
        w->base.unk34 = w->base.unk2E;
    }
    Boss7PartD5_Unk_ov000_022e99e0(w);
    Boss7PartD5_Unk_ov000_022e9990(w, w->base.unk34);
    if (w->unk464 != 0) {
        w->unk464--;
        if (((w->base.unk5C & 1) && (w->unk3DC & 0x10)) || (!(w->base.unk5C & 1) && (w->unk3DC & 0x20))) {
            if (++w->unk466 == 2) {
                w->unk440 = Boss7PartD5_MoveState_Unk_ov000_022e9488;
                return;
            }
        }
    }
    if (w->unk3DC & 4) {
        Boss7PartD5_Unk_ov000_022e85f8(w);
        return;
    }
    if (Boss7PartD5_Unk_ov000_022e9b28(w))
        return;
    if (Boss7PartD5_Unk_ov000_022e9b04(w))
        return;
    if (w->base.unk2E != 0)
        return;
    if (w->unk464 == 0)
        w->unk440 = Boss7PartD5_MoveState_Unk_ov000_022e98d8;
}
