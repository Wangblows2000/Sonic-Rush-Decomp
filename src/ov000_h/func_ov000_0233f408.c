#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_0233f408_Pos;

typedef struct {
    u32 t;
    u8 pad4[0x8 - 4];
    UnkStruct_0233f408_Pos a[2];
    UnkStruct_0233f408_Pos b[2];
    UnkStruct_0233f408_Pos c[2];
    u8 pad38[0x48 - 0x38];
    u8 n1[2];
    u8 n2[2];
} UnkStruct_0233f408;

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
    u8 pad14[0x24 - 0x14];
    u32 unk24;
} UnkStruct_0233f408_7cdc;

typedef struct {
    u16 *t[5];
} UnkStruct_0233f408_Tbl;

typedef struct {
    u32 cur;
    u8 pad[0x10];
} UnkStruct_0233f408_Cur;

typedef struct {
    u8 pad0[0xC8];
    u32 unkC8;
} UnkStruct_0233f408_aad8;

extern s16 data_0206e470[];
extern UnkStruct_0233f408_aad8 data_ov000_0236aad8;
extern UnkStruct_0233f408_7cdc data_02087cdc;
extern UnkStruct_0233f408_Tbl data_02087ce0[];
extern UnkStruct_0233f408_Cur data_02087cec[];
extern u16 *data_02087cf4[];

#define SIN_0233f408(a) data_0206e470[((s32)((u16)(a) & 0xffff) >> 4) * 2]

void func_ov000_0233f408(UnkStruct_0233f408 *w, s32 idx) {
    u8 n;
    u16 x;
    u16 y;
    u8 i;
    u16 *tbl;


    if (data_ov000_0236aad8.unkC8 & 1) {
        if (!(data_ov000_0236aad8.unkC8 & 2)) {
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
        *(u16 *)((u8 *)tbl + i * 4) = x;
        *(u16 *)((u8 *)tbl + (i * 4 + 2)) = y;
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
        *(u16 *)((u8 *)tbl + (i * 4 + 2)) = y - 0x100;
    }
}
