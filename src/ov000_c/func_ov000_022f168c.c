#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x43C - 0x20C];
    s32 unk43C;
    u16 unk440;
    u16 unk442;
    u8 pad444[0x458 - 0x444];
    s32 unk458;
} UnkStruct_022f168c;


extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_StageObj *data_ov000_0236ac60;
extern u16 data_ov000_02365e20[][0x18];
extern s32 data_ov000_02365e60[][3][8];
extern s16 data_ov000_02365da0[][3][8];

void func_ov000_022f4ce4(UnkStruct_022f168c *w, s32 a, s32 b, s32 c);
void *func_ov000_022fad3c(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void func_ov000_022f784c(void *o, s32 a, s32 b, s32 c);
void func_ov000_022f14a8(void);

static inline BOOL IsMode23(void) {
    return data_022c4560.unk10 == 2 || data_022c4560.unk10 == 3;
}

static inline s32 GetLevel(void) {
    return IsMode23() ? 1 : data_022c4560.unk14;
}

void func_ov000_022f168c(UnkStruct_022f168c *w) {
    u16 p;
    void *o;
    s32 x, y, wd;

    func_ov000_022f4ce4(w, 0x27, 0x1000, 1);
    w->unk442 = 0;
    p = 0;
    if (w->unk458 == 2) {
        p |= 1;
        p |= data_ov000_02365e20[GetLevel()][0] << 1;
    }
    o = func_ov000_022fad3c(0xdc, 0, 0, p, 0, 0, 0, 0, 0);
    wd = data_ov000_02365e60[GetLevel()][w->unk458][w->unk440];
    x = data_ov000_0236ac60->unk10 - 0x2000 + ((Rand() & 0x3f) << 8);
    if (w->unk43C != 0)
        x += wd >> 1;
    else
        x -= wd >> 1;
    if (w->unk43C != 0)
        y = x - wd;
    else
        y = x + wd;
    if (x < 0x1800)
        x = 0x1800;
    else if (x > 0x32800)
        x = 0x32800;
    if (y < 0x1800)
        y = 0x1800;
    else if (y > 0x32800)
        y = 0x32800;
    func_ov000_022f784c(o, x, y, data_ov000_02365da0[GetLevel()][w->unk458][w->unk440]);
    w->base.unk68 = func_ov000_022f14a8;
}
