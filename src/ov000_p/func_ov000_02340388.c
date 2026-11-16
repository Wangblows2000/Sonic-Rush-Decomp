#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x2C - 4];
    s32 unk2C;
    s32 unk30;
    u8 pad34[0x40 - 0x34];
    u8 unk40;
    u8 unk41;
} UnkStruct_02340388;

typedef struct {
    u8 pad0[8];
    s32 unk8;
    u8 padC[0x20 - 0xC];
    s16 unk20;
    u8 pad22[0x62 - 0x22];
    u16 unk62;
} UnkPlayer_02340388;

typedef struct {
    u16 ang;
    u16 spd;
    s32 unk4;
    s16 t;
} UnkWave_02340388;

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
    u8 pad14[0x24 - 0x14];
    u32 unk24;
} UnkStruct_02340388_7cdc;

typedef struct {
    u8 pad0[0xC8];
    u32 unkC8;
} UnkStruct_02340388_aad8;

extern s16 data_0206e470[];
extern UnkStruct_02340388_aad8 gCamera;
extern UnkStruct_02340388_7cdc data_02087cdc;
extern u16 *data_02087ce0[];
extern u16 *data_02087cf4[];

#define IDX_02340388(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_02340388(a) data_0206e470[(u32)(IDX_02340388(a) << 1)]
#define COS_02340388(a) data_0206e470[(u32)((IDX_02340388(a) << 1) + 1)]

UnkPlayer_02340388 *Camera_GetBySwapState(void);
UnkPlayer_02340388 *Camera_GetOtherBySwapState(void);
void func_ov000_0233e8ac(UnkWave_02340388 *wave, s32 v, u32 z);

void func_ov000_02340388(UnkStruct_02340388 *w) {
    UnkWave_02340388 wave = {0};
    u16 *tbl;
    UnkPlayer_02340388 *p;
    UnkPlayer_02340388 *q;
    s32 v;
    u16 i;
    u16 x;
    u16 y;
    u8 n;

    if (gCamera.unkC8 & 2) {
        tbl = data_02087ce0[data_02087cdc.unk10];
    } else {
        tbl = data_02087cf4[data_02087cdc.unk24];
    }
    p = Camera_GetBySwapState();
    q = Camera_GetOtherBySwapState();
    if (w->unk40 == 0) {
        v = (p->unk20 * 0x110 << 8) >> 16;
    } else if (w->unk40 < 0xc0) {
        v = (s16)((0xc0 - w->unk40) + ((p->unk20 * 0x50) >> 8));
    } else if (w->unk41 == 0) {
        v = (s16)((u32)(((q->unk8 >> 8) - q->unk62) * q->unk20) >> 8);
    } else {
        v = 0;
    }
    x = w->unk2C >> 8;
    y = w->unk30 >> 8;
    n = w->unk41;
    for (i = 0; i < n; i++) {
        *(u16 *)((u8 *)tbl + i * 4) = x;
        *(u16 *)((u8 *)tbl + (i * 4 + 2)) = y;
    }
    func_ov000_0233e8ac(&wave, v, w->unk0);
    for (; i < 0xc0; i++) {
        wave.ang -= wave.spd;
        if (wave.t % 8 == 0) {
            wave.spd -= wave.spd / 10;
        }
        *(u16 *)((u8 *)tbl + i * 4) = x + (u16)(((0x500 - wave.t) * SIN_02340388(wave.ang)) >> 20);
        *(u16 *)((u8 *)tbl + (i * 4 + 2)) = y + ((u32)(COS_02340388((u16)((wave.t << 11) - (wave.unk4 << ((wave.unk4 << 9) + 10)))) << 5) >> 16);
        wave.t++;
    }
}
