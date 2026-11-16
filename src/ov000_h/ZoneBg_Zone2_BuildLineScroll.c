#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_023405cc_Pos;

typedef struct {
    u32 unk0;
    u8 pad4[0xC - 4];
    UnkStruct_023405cc_Pos a[2];
    UnkStruct_023405cc_Pos b[2];
    u8 pad2C[0x3C - 0x2C];
    u8 n0[2];
    u8 n1[2];
} UnkStruct_023405cc;

typedef struct {
    u16 ang;
    u16 spd;
    s32 unk4;
    s16 t;
} UnkWave_023405cc;

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
    u8 pad14[0x24 - 0x14];
    u32 unk24;
} UnkStruct_023405cc_7cdc;

typedef struct {
    u16 *t[5];
} UnkStruct_023405cc_Tbl;

typedef struct {
    u32 cur;
    u8 pad[0x10];
} UnkStruct_023405cc_Cur;

typedef struct {
    u8 pad0[0xC8];
    u32 unkC8;
} UnkStruct_023405cc_aad8;

extern s16 data_0206e470[];
extern UnkStruct_023405cc_aad8 gCamera;
extern UnkStruct_023405cc_7cdc data_02087cdc;
extern UnkStruct_023405cc_Tbl data_02087ce0[];
extern UnkStruct_023405cc_Cur data_02087cec[];
extern u16 *data_02087cf4[];

#define IDX_023405cc(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_023405cc(a) data_0206e470[(u32)(IDX_023405cc(a) << 1)]
#define COS_023405cc(a) data_0206e470[(u32)((IDX_023405cc(a) << 1) + 1)]

void ZoneBg_Zone2_Unk_ov000_0233e8ac(UnkWave_023405cc *wave, s32 v, u32 z);

void ZoneBg_Zone2_BuildLineScroll(UnkStruct_023405cc *w, s32 idx) {
    UnkWave_023405cc wave = {0};
    u16 *tbl;
    u16 i;
    u16 x;
    u16 y;
    u8 n;

    if (gCamera.unkC8 & 1) {
        if (!(gCamera.unkC8 & 2)) {
            tbl = data_02087ce0[0].t[data_02087cdc.unk10];
        } else {
            tbl = data_02087cf4[data_02087cdc.unk24];
        }
    } else {
        tbl = data_02087ce0[idx].t[data_02087cec[idx].cur];
    }

    x = w->a[idx].x >> 8;
    y = w->a[idx].y >> 8;
    n = w->n0[idx];
    for (i = 0; i < n; i++) {
        *(u16 *)((u8 *)tbl + i * 4) = x;
        *(u16 *)((u8 *)tbl + (i * 4 + 2)) = y;
    }

    x = w->b[idx].x >> 8;
    y = w->b[idx].y >> 8;
    n = w->n1[idx];
    for (; i < n; i++) {
        *(u16 *)((u8 *)tbl + i * 4) = x + (u16)(((0x500 - wave.t) * SIN_023405cc(wave.ang)) >> 20);
        *(u16 *)((u8 *)tbl + (i * 4 + 2)) = y + ((u32)(COS_023405cc((u16)((wave.t << 11) - (wave.unk4 << ((wave.unk4 << 9) + 10)))) << 5) >> 16);
    }

    ZoneBg_Zone2_Unk_ov000_0233e8ac(&wave, 0, w->unk0);
    x = w->b[idx].x >> 8;
    y = w->b[idx].y >> 8;
    for (; i < 0xc0; i++) {
        wave.ang -= wave.spd;
        if (wave.t % 8 == 0) {
            wave.spd -= wave.spd / 10;
        }
        *(u16 *)((u8 *)tbl + i * 4) = x + (u16)(((0x500 - wave.t) * SIN_023405cc(wave.ang)) >> 20);
        *(u16 *)((u8 *)tbl + (i * 4 + 2)) = y + ((u32)(COS_023405cc((u16)((wave.t << 11) - (wave.unk4 << ((wave.unk4 << 9) + 10)))) << 5) >> 16);
        wave.t++;
    }
}
