#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_0233fc44_Pos;

typedef struct {
    u8 pad0[0x3C];
    s32 x;
    s32 y;
} UnkStruct_0233fc44;

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
    u8 pad14[0x24 - 0x14];
    u32 unk24;
} UnkStruct_0233fc44_7cdc;

typedef struct {
    u16 *t[5];
} UnkStruct_0233fc44_Tbl;

typedef struct {
    u32 cur;
    u8 pad[0x10];
} UnkStruct_0233fc44_Cur;

typedef struct {
    u8 pad0[0xC8];
    u32 unkC8;
} UnkStruct_0233fc44_aad8;

extern s16 data_0206e470[];
extern UnkStruct_0233fc44_aad8 gCamera;
extern UnkStruct_0233fc44_7cdc data_02087cdc;
extern UnkStruct_0233fc44_Tbl data_02087ce0[];
extern UnkStruct_0233fc44_Cur data_02087cec[];
extern u16 *data_02087cf4[];

#define SIN_0233fc44(a) data_0206e470[((s32)((u16)(a) & 0xffff) >> 4) * 2]

void func_ov000_0233fc44(UnkStruct_0233fc44 *w) {
    u8 n;
    u8 i;
    u16 x;
    u16 y;
    u16 *tbl;

    if (gCamera.unkC8 & 2) {
        tbl = data_02087ce0[0].t[data_02087cdc.unk10];
    } else {
        tbl = data_02087cf4[data_02087cdc.unk24];
    }
    x = w->x >> 8;
    y = w->y >> 8;
    if (0x200 - y < 0xC0) {
        n = 0x200 - y;
    } else {
        n = 0xC0;
    }
    for (i = 0; i < n; i++) {
        *(u16 *)((u8 *)tbl + i * 4) = x;
        *(u16 *)((u8 *)tbl + (i * 4 + 2)) = y;
    }
    for (; i < 0xC0; i++) {
        *(u16 *)((u8 *)tbl + i * 4) = x;
        *(u16 *)((u8 *)tbl + (i * 4 + 2)) = y - 0x118;
    }
}
