#include <nitro/types.h>

typedef struct UnkStruct_022fb28c_o {
    u8 pad0[0x18];
    s32 x;
} UnkStruct_022fb28c_o;

typedef struct UnkStruct_022fb28c {
    u8 pad0[0x14];
    u32 flags;
    s32 x;
    s32 y;
    u8 pad20[0x50 - 0x20];
    s32 sx;
    u8 pad54[0x5c - 0x54];
    s32 vx;
    u8 pad60[0x74 - 0x60];
    void (*state)(struct UnkStruct_022fb28c *p);
    u8 pad78[0x80 - 0x78];
    UnkStruct_022fb28c_o *target;
    u8 pad84[0x1cc - 0x84];
    s32 unk1CC;
} UnkStruct_022fb28c;

typedef struct UnkStruct_022c4560 {
    u8 pad0[0x10];
    s32 mode;
    s32 level;
} UnkStruct_022c4560;

typedef struct UnkStruct_022fb28c_c {
    u8 pad0[0x40];
    s32 chr;
} UnkStruct_022fb28c_c;

extern UnkStruct_022c4560 data_022c4560;
extern s32 data_ov001_0231d664[][5];

BOOL func_ov001_02300b84(s32 x, s32 y, s32 w, s32 h);
UnkStruct_022fb28c_c *ExtraBoss_GetWork(void);
void func_ov001_022fb17c(UnkStruct_022fb28c *p);

static inline s32 GetLevel(void) {
    BOOL b = TRUE;
    if (data_022c4560.mode != 2 && data_022c4560.mode != 3)
        b = FALSE;
    if (b)
        return 1;
    return data_022c4560.level;
}

void func_ov001_022fb28c(UnkStruct_022fb28c *p) {
    UnkStruct_022fb28c_o *t = p->target;

    if (func_ov001_02300b84(p->x, p->y, 0x30000, 0x30000)) {
        p->flags |= 8;
        return;
    }
    if (p->sx + p->vx >= data_ov001_0231d664[GetLevel()][ExtraBoss_GetWork()->chr]) {
        p->sx = data_ov001_0231d664[GetLevel()][ExtraBoss_GetWork()->chr];
        p->vx = 0;
    }
    if (t->x < p->x - 0xa000)
        return;
    if (t->x > p->x + 0xa000)
        return;
    p->state = func_ov001_022fb17c;
    p->sx = p->vx = 0;
    p->unk1CC = 10;
}
