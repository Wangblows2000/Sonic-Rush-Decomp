#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    u8 padC[0x2C - 0xC];
    s32 unk2C;
    s32 unk30;
    u32 unk34;
    u8 pad38[0x64 - 0x38];
} UnkPlayer_02340be4;

typedef struct {
    UnkPlayer_02340be4 p0;
    u8 pad64[0xC8 - 0x64];
    u32 unkC8;
} UnkPlayers_02340be4;

typedef struct {
    u8 pad0[0x10];
    u16 unk10;
} UnkStruct_02340be4_abd8;

extern UnkPlayers_02340be4 gCamera;
extern UnkPlayer_02340be4 data_ov000_0236ab3c;
extern UnkStruct_02340be4_abd8 data_ov000_0236abd8;

UnkPlayer_02340be4 *Camera_GetBySwapState(void);
UnkPlayer_02340be4 *Camera_GetOtherBySwapState(void);
s32 FX_Div(s32 a, s32 b);

void ZoneBg_Zone4_Update(void) {
    UnkPlayer_02340be4 *p;
    UnkPlayer_02340be4 *q;

    if (gCamera.unkC8 & 1) {
        p = Camera_GetBySwapState();
        q = Camera_GetOtherBySwapState();
        if (!(p->unk34 & 1)) {
            p->unk2C = p->unk4 / 18;
            p->unk30 = FX_Div(p->unk8 * 0x30, (data_ov000_0236abd8.unk10 - 0xc0) << 8) >> 4;
        }
        if (q->unk34 & 1) {
            return;
        }
        q->unk2C = q->unk4 / 18;
        q->unk30 = (FX_Div(p->unk8 * 0x30, (data_ov000_0236abd8.unk10 - 0xc0) << 8) >> 4) + 0x11000;
        return;
    }
    p = &gCamera.p0;
    if (!(p->unk34 & 1)) {
        p->unk2C = p->unk4 / 18;
        p->unk30 = FX_Div(p->unk8 * 0x30, (data_ov000_0236abd8.unk10 - 0xc0) << 8) >> 4;
    }
    p = &data_ov000_0236ab3c;
    if (p->unk34 & 1) {
        return;
    }
    p->unk2C = p->unk4 / 18;
    p->unk30 = FX_Div(p->unk8 * 0x30, (data_ov000_0236abd8.unk10 - 0xc0) << 8) >> 4;
}
