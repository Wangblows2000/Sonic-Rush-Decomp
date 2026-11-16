#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_0233fd18_Pos;

typedef struct {
    u32 t;
    u8 pad4[0xC - 4];
    UnkStruct_0233fd18_Pos a[2];
    UnkStruct_0233fd18_Pos b[2];
    UnkStruct_0233fd18_Pos c[2];
    u8 pad3C[0x4C - 0x3C];
    u8 n1[2];
    u8 n2[2];
} UnkStruct_0233fd18;

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
    u8 pad14[0x24 - 0x14];
    u32 unk24;
} UnkStruct_0233fd18_7cdc;

typedef struct {
    u16 *t[5];
} UnkStruct_0233fd18_Tbl;

typedef struct {
    u32 cur;
    u8 pad[0x10];
} UnkStruct_0233fd18_Cur;

typedef struct {
    u8 pad0[0xC8];
    u32 unkC8;
} UnkStruct_0233fd18_aad8;

extern s16 data_0206e470[];
extern UnkStruct_0233fd18_aad8 gCamera;
extern UnkStruct_0233fd18_7cdc data_02087cdc;
extern UnkStruct_0233fd18_Tbl data_02087ce0[];
extern UnkStruct_0233fd18_Cur data_02087cec[];
extern u16 *data_02087cf4[];

#define SIN_0233fd18(a) data_0206e470[((s32)((u16)(a) & 0xffff) >> 4) * 2]

void func_ov000_0233fd18(UnkStruct_0233fd18 *w, s32 idx) {
    u32 t;
    u16 x;
    u16 y;
    u8 n;
    u16 *tbl;
    u8 i;

    t = w->t;

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
    n = w->n1[idx];
    for (i = 0; i < n; i++) {
        s32 c = (s32)(i << 16) / 24;
        *(u16 *)((u8 *)tbl + i * 4) = x + (SIN_0233fd18((u32)(t << 16) / 96 - c) >> 11);
        *(u16 *)((u8 *)tbl + (i * 4 + 2)) = y + (SIN_0233fd18((u32)(t << 16) / 144 - c) >> 12);
    }

    x = w->b[idx].x >> 8;
    y = w->b[idx].y >> 8;
    n = w->n2[idx];
    for (; i < n; i++) {
        *(u16 *)((u8 *)tbl + i * 4) = x;
        *(u16 *)((u8 *)tbl + (i * 4 + 2)) = y;
    }

    y = w->c[idx].y >> 8;
    x = w->c[idx].x >> 8;
    if (0x200 - y < 0xC0) {
        n = 0x200 - y;
    } else {
        n = 0xC0;
    }
    for (; i < n; i++) {
        *(u16 *)((u8 *)tbl + i * 4) = x;
        *(u16 *)((u8 *)tbl + (i * 4 + 2)) = y;
    }
    for (; i < 0xC0; i++) {
        *(u16 *)((u8 *)tbl + i * 4) = x;
        *(u16 *)((u8 *)tbl + (i * 4 + 2)) = y - 0x118;
    }
}
