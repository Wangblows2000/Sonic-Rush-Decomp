#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2E];
    s16 unk2E;
    u8 pad30[0x1F0 - 0x30];
    s16 unk1F0;
    u8 pad1F2[0x3D0 - 0x1F2];
    s32 unk3D0;
    u8 pad3D4[0x3DC - 0x3D4];
    u16 unk3DC;
    u8 pad3DE[0x440 - 0x3DE];
    void *unk440;
} UnkStruct_022e97cc;

typedef struct {
    u8 pad[8];
    u16 unk8;
} UnkStruct_02369ac0;

extern s16 data_ov000_02368b80[][0x18];
extern UnkStruct_02369ac0 gBossStatus;

void func_ov000_022e85f8(UnkStruct_022e97cc *w);
BOOL func_ov000_022e9b28(UnkStruct_022e97cc *w);
BOOL func_ov000_022e9b4c(UnkStruct_022e97cc *w);
BOOL func_ov000_022e9b04(UnkStruct_022e97cc *w);
void func_ov000_022e75b0(void);

void Boss7PartD5_MoveState_Unk_ov000_022e97cc(UnkStruct_022e97cc *w) {
    s32 v = w->unk2E;
    if (v < 0) {
        if (data_ov000_02368b80[w->unk3D0][0] < -v)
            w->unk2E = v + data_ov000_02368b80[w->unk3D0][0];
        else
            w->unk2E = 0;
    } else if (v > 0) {
        if (data_ov000_02368b80[w->unk3D0][0] < v)
            w->unk2E = v - data_ov000_02368b80[w->unk3D0][0];
        else
            w->unk2E = 0;
    }
    if (w->unk3DC & 4) {
        func_ov000_022e85f8(w);
        return;
    }
    if (func_ov000_022e9b28(w))
        return;
    if (func_ov000_022e9b4c(w))
        return;
    if (func_ov000_022e9b04(w))
        return;
    if (gBossStatus.unk8 > 1)
        return;
    if (w->unk1F0 == 0)
        w->unk440 = func_ov000_022e75b0;
}
