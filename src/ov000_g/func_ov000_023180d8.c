#include <nitro/types.h>

typedef struct {
    s32 x;
    s32 y;
} UnkStruct_023180d8_P;

typedef struct {
    u8 pad0[6];
    s8 unk6;
} UnkStruct_023180d8_Prm;

typedef struct {
    u8 unk0;
    u8 unk1;
    u8 pad2[0xC - 2];
    s32 unkC;
    s32 unk10;
    s32 unk14;
    u8 pad18[0x1C - 0x18];
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x54 - 0x24];
    u32 unk54;
    u8 pad58[0x5C - 0x58];
    u16 unk5C;
    u8 pad5E[0x94 - 0x5E];
    u8 unk94[0xDC - 0x94];
    u8 unkDC[0x1E0 - 0xDC];
    UnkStruct_023180d8_Prm *unk1E0;
    u8 pad1E4[0x20C - 0x1E4];
    u8 unk20C[0x254 - 0x20C];
    u8 unk254[0x29C - 0x254];
    u8 unk29C[0x2E4 - 0x29C];
    u8 unk2E4[0x32C - 0x2E4];
    u8 unk32C[0x374 - 0x32C];
    u8 unk374[0x3BC - 0x374];
    u8 unk3BC[0x404 - 0x3BC];
    u8 unk404[0x44C - 0x404];
    s32 unk44C;
    s32 unk450;
    u8 pad454[0x478 - 0x454];
    s16 unk478[0x14];
    u16 unk4A0;
    s16 unk4A2;
    u32 unk4A4;
} UnkStruct_023180d8;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_023180d8 *work;
} UnkStruct_023180d8_Task;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_023180d8_Sys;

extern UnkStruct_023180d8_Task *gTaskCurrent;
extern UnkStruct_023180d8_Sys data_022c4560;
extern s16 data_0206e470[];

s16 ObjSpd_DownSet(s16 a, s32 b);
void StageObj_Draw2D(void *w, void *a, void *b);
void StageObj_Draw2DEx(s32 x, s32 y, u8 rot, void *w, void *a, void *b, s32 c);
void StageObj_HandleCollider(void *w, void *a);

#define SIN_IDX(i) data_0206e470[(u32)((i) << 1)]
#define COS_IDX(i) data_0206e470[(u32)(((i) << 1) + 1)]

void func_ov000_023180d8(void) {
    UnkStruct_023180d8_P pos[20];
    UnkStruct_023180d8 *w;
    s32 n;
    s16 i;
    s16 j;
    s32 bx;
    s32 by;
    s32 len;
    s16 k;
    s32 idx;
    s32 p;

    k = 0x60;
    w = gTaskCurrent->work;
    n = w->unk1E0->unk6;
    if (n > 10) {
        k += (n - 10) * 4;
    }
    for (i = 0; i <= n; i++) {
        if (i != 0) {
            bx = pos[i - 1].x;
            by = pos[i - 1].y;
            len = 0x800;
            w->unk478[i] = w->unk478[i] + ((k * (w->unk478[i - 1] - w->unk478[i])) >> 8);
        } else {
            bx = w->unk10;
            by = w->unk14;
            len = 0x2000;
            w->unk478[i] = w->unkC;
        }
        idx = (s32)((u16)w->unk478[i] & 0xffff) >> 4;
        pos[i].x = bx + ((len * SIN_IDX(idx)) >> 12);
        pos[i].y = by + ((len * COS_IDX(idx)) >> 12);
    }
    w->unk4A0 += w->unk4A2;
    w->unk4A2 = ObjSpd_DownSet(w->unk4A2, 2);
    w->unk1 = w->unk4A0 >> 8;
    w->unk5C |= 0x100;
    StageObj_Draw2D(w, w->unk94, w->unkDC);
    w->unk5C &= ~0x100;
    w->unk1 = 0;
    w->unk5C |= 4;
    StageObj_Draw2D(w, w->unk3BC, w->unk404);
    w->unk5C &= ~4;
    for (j = 0; j <= n; j++) {
        if (j == n) {
            w->unk5C |= 0x100;
            w->unk1 = -(w->unk478[j] >> 8);
            StageObj_Draw2DEx(pos[j].x, pos[j].y, w->unk1, w, w->unk32C, w->unk374, 0);
        } else if (w->unk4A4 & (1 << j)) {
            StageObj_Draw2DEx(pos[j].x, pos[j].y, 0, NULL, w->unk20C, w->unk254, 0);
        } else {
            StageObj_Draw2DEx(pos[j].x, pos[j].y, 0, NULL, w->unk29C, w->unk2E4, 0);
        }
    }
    w->unk5C &= ~0x100;
    p = n - 1;
    idx = (s32)((u16)w->unk478[n] & 0xffff) >> 4;
    pos[n].x = pos[p].x + ((SIN_IDX(idx) * 0x1a00) >> 12);
    pos[n].y = pos[p].y + ((COS_IDX(idx) * 0x1a00) >> 12);
    if (!(w->unk54 & 2)) {
        StageObj_HandleCollider(w, &w->unk44C);
        w->unk44C = pos[n].x >> 8;
        w->unk450 = pos[n].y >> 8;
    }
    idx = (s32)((u16)w->unk478[n] & 0xffff) >> 4;
    pos[n].x = pos[p].x + ((SIN_IDX(idx) * 0x2400) >> 12);
    pos[n].y = pos[p].y + ((COS_IDX(idx) * 0x2400) >> 12);
    w->unk1C = pos[n].x;
    w->unk20 = pos[n].y;
    if (!(data_022c4560.unk20 & 3)) {
        w->unk4A4 = (w->unk4A4 << 1) | ((w->unk4A4 & 0x20000000) >> 29);
    }
}
