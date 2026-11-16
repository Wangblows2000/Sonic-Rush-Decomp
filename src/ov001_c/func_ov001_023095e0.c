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

extern UnkStruct_023095e0 *gSpecialStageHud;
extern UnkStruct_023095e0_br gDisplayMain;

BOOL func_0202e5e4(float a, float b);
BOOL func_0202e528(float a, float b);
u32 _ffixu(float a);

#define REG_DISPCNT (*(vu32 *)0x4000000)
#define REG_BG0CNT (*(vu16 *)0x4000008)
#define REG_BG1CNT (*(vu16 *)0x400000a)
#define REG_BG2CNT (*(vu16 *)0x400000c)
#define REG_BG3CNT (*(vu16 *)0x400000e)

void func_ov001_023095e0(void) {
    UnkStruct_023095e0 *g = gSpecialStageHud;
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
    gDisplayMain.b1 = 1;
    gDisplayMain.b8 = 1;
    gDisplayMain.b9 = 1;
    gDisplayMain.e0 = 4;
    gDisplayMain.b10 = 1;
    gDisplayMain.b11 = 1;
    gDisplayMain.b6 = 1;
    gDisplayMain.e8 = 0xf - gDisplayMain.e0;
    if (g->timer >= 0x168 && g->timer < 0x172) {
        f = (g->timer - 0x168) / 10.0f;
        if (func_0202e5e4(f, 1.0f))
            f = 1.0f;
        gDisplayMain.e0 = (u16)(_ffixu(4.0f * f) & 0xf);
        gDisplayMain.e8 = (u16)((0xf - gDisplayMain.e0) & 0xf);
    } else if (g->timer >= 0x258) {
        f = 1.0f - (g->timer - 0x258) / 10.0f;
        if (func_0202e528(f, 0.0f))
            f = 0.0f;
        gDisplayMain.e0 = (u16)(_ffixu(4.0f * f) & 0xf);
        gDisplayMain.e8 = (u16)((0xf - gDisplayMain.e0) & 0xf);
    }
}
