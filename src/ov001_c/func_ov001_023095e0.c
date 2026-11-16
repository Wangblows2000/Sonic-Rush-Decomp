#include <nitro/types.h>

typedef struct UnkStruct_023095e0_br {
    u8 pad0[0x20];
    u16 b0 : 1;
    u16 b1 : 1;
    u16 p2 : 4;
    u16 b6 : 2;
    u16 b8 : 1;
    u16 b9 : 1;
    u16 b10 : 1;
    u16 b11 : 1;
    u16 p12 : 4;
    u16 e0 : 5;
    u16 p5 : 3;
    u16 e8 : 5;
    u16 p13 : 3;
} UnkStruct_023095e0_br;

typedef struct UnkStruct_023095e0 {
    u8 pad0[0xae0];
    s32 timer;
} UnkStruct_023095e0;

extern UnkStruct_023095e0 *data_ov001_0231f5bc;
extern UnkStruct_023095e0_br data_0208922c;

BOOL func_0202e5e4(float a, float b);
BOOL func_0202e528(float a, float b);
u32 func_0202f17c(float a);

#define REG_DISPCNT (*(vu32 *)0x4000000)
#define REG_BG0CNT (*(vu16 *)0x4000008)
#define REG_BG1CNT (*(vu16 *)0x400000a)
#define REG_BG2CNT (*(vu16 *)0x400000c)
#define REG_BG3CNT (*(vu16 *)0x400000e)

void func_ov001_023095e0(void) {
    UnkStruct_023095e0 *g = data_ov001_0231f5bc;
    float f;

    REG_BG1CNT = REG_BG1CNT & ~3;
    REG_BG0CNT = (REG_BG0CNT & ~3) | 1;
    REG_BG2CNT = (REG_BG2CNT & ~3) | 2;
    REG_BG3CNT = (REG_BG3CNT & ~3) | 3;
    if (g->timer > 0x262) {
        u32 v = (REG_DISPCNT & 0x1f00) >> 8;
        REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | ((v & ~2) << 8);
    } else if (g->timer > 0x168) {
        u32 v = (REG_DISPCNT & 0x1f00) >> 8;
        REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | ((v | 2) << 8);
    }
    data_0208922c.b1 = 1;
    data_0208922c.b8 = 1;
    data_0208922c.b9 = 1;
    data_0208922c.e0 = 4;
    data_0208922c.b10 = 1;
    data_0208922c.b11 = 1;
    data_0208922c.b6 = 1;
    data_0208922c.e8 = 0xf - data_0208922c.e0;
    if (g->timer >= 0x168 && g->timer < 0x172) {
        f = (g->timer - 0x168) / 10.0f;
        if (func_0202e5e4(f, 1.0f))
            f = 1.0f;
        data_0208922c.e0 = (u16)(func_0202f17c(4.0f * f) & 0xf);
        data_0208922c.e8 = (u16)((0xf - data_0208922c.e0) & 0xf);
    } else if (g->timer >= 0x258) {
        f = 1.0f - (g->timer - 0x258) / 10.0f;
        if (func_0202e528(f, 0.0f))
            f = 0.0f;
        data_0208922c.e0 = (u16)(func_0202f17c(4.0f * f) & 0xf);
        data_0208922c.e8 = (u16)((0xf - data_0208922c.e0) & 0xf);
    }
}
