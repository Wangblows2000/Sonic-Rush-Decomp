#include <nitro/types.h>

typedef struct {
    s8 dx;
    s8 dy;
    s8 dz : 4;
    u8 ang : 4;
    u8 state;
} UnkRec_02344f68;

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x10 - 2];
    s32 x;
    s32 y;
    s32 z;
    u8 pad1C[0x5C - 0x1C];
    u16 unk5C;
    u8 pad5E[0x325 - 0x5E];
    u8 unk325;
    u8 pad326[0x328 - 0x326];
    s16 unk328;
} UnkStruct_02344f68;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 pad1C[0x20 - 0x1C];
    u32 unk20;
} UnkStruct_02344f68_4560;

typedef struct {
    u8 pad0[0x54];
    UnkRec_02344f68 *unk54;
} UnkStruct_02344f68_4590;

extern UnkStruct_02344f68_4560 gGameState;
extern UnkStruct_02344f68_4590 data_022c4590;
extern s32 data_ov000_0236ac2c;
extern s32 data_ov000_0236ac30;
extern s32 data_ov000_0236ac24;

void Player_RecordReplayFrame(UnkStruct_02344f68 *w) {
    UnkRec_02344f68 *recs;
    u32 idx;
    s16 d;

    if (w->unk325 != 0) {
        return;
    }
    if ((gGameState.unk20 >> 2) >= 0xc4d) {
        return;
    }
    if (!(gGameState.unk18 & 1)) {
        return;
    }
    if (gGameState.unk20 & 3) {
        return;
    }
    idx = (gGameState.unk20 - 1) >> 2;
    recs = data_022c4590.unk54;
    d = (w->x >> 8) - data_ov000_0236ac2c;
    d = d < -0x80 ? -0x80 : (d > 0x7f ? 0x7f : d);
    recs[idx].dx = d;
    data_ov000_0236ac2c += d;
    d = (w->y >> 8) - data_ov000_0236ac30;
    d = d < -0x80 ? -0x80 : (d > 0x7f ? 0x7f : d);
    recs[idx].dy = d;
    data_ov000_0236ac30 += d;
    d = (w->z >> 8) - data_ov000_0236ac24;
    d = d < -0x10 ? -0x10 : (d > 0xf ? 0xf : d);
    recs[idx].dz = d;
    data_ov000_0236ac24 += (s8)d;
    recs[idx].ang = w->unk1 >> 4;
    recs[idx].state = w->unk328;
    recs[idx].state |= (w->unk5C & 1) << 7;
    recs[idx + 1].state = 0xff;
}
