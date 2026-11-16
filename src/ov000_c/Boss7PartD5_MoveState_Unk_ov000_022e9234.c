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
} UnkStruct_022e9234;

extern s16 data_ov000_02368b80[][0x18];
extern s16 gPlayerPhysicsTable[][0x18];

void Boss7PartD5_MoveState_Unk_ov000_022e98d8(void);
void func_ov000_022e8c90(void);

void Boss7PartD5_MoveState_Unk_ov000_022e9234(UnkStruct_022e9234 *w) {
    u16 f = w->unk3D8;

    if (!(f & 0x30)) {
        s32 v = w->base.unk2E;
        if (v < 0) {
            if (data_ov000_02368b80[w->unk3D0][0] < -v)
                w->base.unk2E = v + data_ov000_02368b80[w->unk3D0][0];
            else
                w->base.unk2E = 0;
        } else if (v > 0) {
            if (data_ov000_02368b80[w->unk3D0][0] < v)
                w->base.unk2E = v - data_ov000_02368b80[w->unk3D0][0];
            else
                w->base.unk2E = 0;
        }
    } else {
        s16 a = gPlayerPhysicsTable[w->unk3D0][0];
        s32 lim = w->unk430;
        if (f & 0x20)
            w->base.unk5C |= 1;
        else if (f & 0x10)
            w->base.unk5C &= ~1;
        if (w->base.unk5C & 1)
            a = -a;
        w->base.unk2E += a;
        w->base.unk2E = w->base.unk2E < (s16)-lim ? (s16)-lim : (w->base.unk2E > (s16)lim ? (s16)lim : w->base.unk2E);
        w->base.unk34 = w->base.unk2E;
    }
    if (w->base.unk58 & 1) {
        w->base.unk30 = 0;
        w->base.unk58 &= ~0x10;
        w->unk440 = Boss7PartD5_MoveState_Unk_ov000_022e98d8;
        return;
    }
    if (w->unk3DC & 0x100)
        w->unk440 = func_ov000_022e8c90;
}
