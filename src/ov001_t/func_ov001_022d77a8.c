#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
} UnkPos_022d77a8;

typedef struct {
    u16 pal[3];
    UnkPos_022d77a8 pos[3];
    u8 pad12[2];
    u32 prio[3];
} UnkStruct_022d77a8;

typedef struct {
    void *destA;
    void *destB;
    u16 *srcA;
    u16 *srcB;
    u8 pad10[0x84 - 0x10];
} UnkLayer_022d77a8;

typedef struct {
    u8 pad0[0x120];
    UnkLayer_022d77a8 layers[3];
    u8 pad[0x800 - 0x120 - 3 * 0x84];
    u16 pal[3];
} UnkWork_022d77a8;

typedef struct {
    u16 x;
    u16 y;
} UnkScroll_022d77a8;

extern UnkScroll_022d77a8 data_0208922c[];
extern UnkScroll_022d77a8 data_02087d04[];

void func_02035704(u16 *src, u16 count, void *dest, u16 *pal);

#pragma thumb on
void func_ov001_022d77a8(UnkStruct_022d77a8 *a, UnkWork_022d77a8 *b) {
    s32 order[3];
    s32 i, j, k;
    s32 idx;
    s32 x, y;

    for (i = 0; i < 3; i++) {
        order[i] = -1;
        for (j = 0; j < 3; j++) {
            for (k = 0; k < i; k++) {
                if (j == order[k]) break;
            }
            if (k < i) continue;
            if (order[i] < 0 || a->prio[order[i]] > a->prio[j]) {
                order[i] = j;
            }
        }
    }
    for (i = 0; i < 3; i++) {
        idx = order[i];
        x = a->pos[idx].x;
        y = a->pos[idx].y;
        b->pal[idx] = a->pal[idx];
        func_02035704(&b->pal[idx], 1, b->layers[idx].destA, b->layers[idx].srcA + 1);
        func_02035704(&b->pal[idx], 1, b->layers[idx].destB, b->layers[idx].srcB + 1);
        if (x < 0) {
            x += (-x / 512 + 1) * 512;
        }
        if (y < 0) {
            y += (-y / 512 + 1) * 512;
        }
        x %= 512;
        y %= 512;
        x = 512 - x;
        y = 512 - y;
        data_0208922c[idx].x = x;
        data_0208922c[idx].y = y;
        data_02087d04[idx].x = x;
        data_02087d04[idx].y = y;
    }
}
#pragma thumb reset
