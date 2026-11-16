#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[0x44];
    u16 flags5c;
    u8 pad5e[0x12];
    u32 unk70;
    u8 pad74[0x1d8];
    s32 arr24c[5];
    u8 pad260[0xcc];
    u32 flags32c;
} UnkObj_0205aae4;

typedef struct {
    u32 pad0;
    UnkObj_0205aae4 *obj;
    u32 unk8;
    u8 padc[0x468];
    void (*fn474)(void);
    u8 pad478[8];
    s32 x480;
    s32 y484;
    u32 flags488;
    u32 flags48c;
    u8 pad490[0x43e];
    u16 unk8ce;
} UnkWork_0205aae4;

typedef struct {
    u32 unk0;
    u8 pad4[0x14];
    u32 flags;
    u8 zone;
    u8 act;
} UnkState_0205aae4;

typedef struct {
    u8 pad0[0x20];
    u16 b0 : 1;
    u16 b1 : 1;
    u16 b2 : 1;
    u16 b3 : 1;
    u16 b4 : 1;
    u16 b5 : 1;
    u16 b6 : 2;
    u16 b8 : 1;
    u16 b9 : 1;
    u16 b10 : 1;
    u16 b11 : 1;
    u16 b12 : 1;
    u16 b13 : 1;
    u16 b14 : 2;
    u8 pad22[0x26];
    s16 bright;
} UnkDisp_0205aae4;

typedef struct {
    u32 unk0;
    u8 pad4[0x30];
    u32 unk34;
    u8 pad38[0x2c];
    u32 unk64;
    u8 pad68[0x30];
    u32 unk98;
    u8 pad9c[0x2c];
    u32 unkc8;
} UnkGfx_0205aae4;

typedef struct {
    u32 flags;
} UnkG_0205aae4;

extern UnkState_0205aae4 gGameState;
extern UnkGfx_0205aae4 gCamera;
extern UnkG_0205aae4 gBossStatus;
extern UnkDisp_0205aae4 gDisplayMain;
extern UnkDisp_0205aae4 gDisplaySub;
extern void *data_02072c80;

void func_0205e218(UnkWork_0205aae4 *w);
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);
void ExtraBoss_StartBattle(void);
void NaviMsg_Create(s32 a, u32 b);
void func_02053b04(BOOL on);
void NaviB_Create(void);
void Sound_PlayVoice(s32 a, s32 b, s32 c);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_0205aacc(void);

#define REG_DISPCNT (*(vu32 *)0x04000000)
#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

static inline void SetBlend_0205aae4(void) {
    gDisplayMain.b0 = 1;
    gDisplayMain.b1 = 0;
    gDisplayMain.b2 = 0;
    gDisplayMain.b3 = 0;
    gDisplayMain.b4 = 0;
    gDisplayMain.b5 = 0;
    gDisplayMain.b6 = 1;
    gDisplayMain.b8 = 0;
    gDisplayMain.b9 = 1;
    gDisplayMain.b10 = 1;
    gDisplayMain.b11 = 1;
    gDisplayMain.b12 = 1;
    gDisplayMain.b13 = 1;
}

static inline void Bright_0205aae4(void) {
    gDisplayMain.bright = 0;
    gDisplaySub.bright = 0x10;
    GXx_SetMasterBrightness_((vu16 *)0x0400006c, gDisplayMain.bright);
    GXx_SetMasterBrightness_((vu16 *)0x0400106c, gDisplaySub.bright);
}

void func_0205aae4(UnkWork_0205aae4 *w) {
    BOOL b;

    w->flags488 &= 0xfff7555b;
    w->flags48c &= ~0x1ff;
    w->flags488 |= 0x20000;
    w->flags48c |= 0x200;
    w->flags488 |= 0x20000000;
    w->obj->unk70 = w->unk8;
    w->obj->flags5c |= 0x40;
    if (gGameState.zone != 8 || gGameState.act != 2) {
        if ((w->flags488 & 0x800000) == 0)
            w->obj->x = w->x480;
        else
            w->obj->y = w->y484;
        w->obj->flags32c &= ~0x600000;
        gGameState.flags |= 1;
        gCamera.unk34 &= ~1;
        gCamera.unk98 &= ~1;
        if (gGameState.zone == 1)
            gCamera.unkc8 &= ~0x2000000;
        gCamera.unkc8 &= ~0x800;
        {
            UnkObj_0205aae4 *o = w->obj;
            u8 i;
            for (i = 0; i < 5; i++)
                o->arr24c[i] = 0x1000;
        }
    }
    func_0205e218(w);

    b = FALSE;
    if (gGameState.zone == 8 && gGameState.act == 2)
        b = TRUE;
    if (b) {
        Bright_0205aae4();
        SetBlend_0205aae4();
        ExtraBoss_StartBattle();
    } else if (gGameState.zone >= 9 || gGameState.act >= 2) {
        gDisplayMain.bright = 0x10;
        gBossStatus.flags |= 1;
        REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | 0x1100;
        REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | 0x1300;
        if (gGameState.zone != 8 || gGameState.act != 2) {
            NaviMsg_Create(1, gGameState.unk0);
            func_02053b04(1);
            NaviB_Create();
        }
    } else {
        u32 plane;
        Bright_0205aae4();
        gCamera.unk0 |= 3;
        gCamera.unk64 |= 3;
        plane = (REG_DISPCNT & 0x1f00) >> 8;
        REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | ((plane & ~4) << 8);
        *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & 0x43) | 0x8010;
        {
            vu16 z = 0;
            MIi_CpuClear16(z, data_02072c80, 0x2000);
        }
        plane = (REG_DISPCNT & 0x1f00) >> 8;
        REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | ((plane | 4) << 8);
        *(vu16 *)0x0400100c = (*(vu16 *)0x0400100c & 0x43) | 0x8010;
        *(vu16 *)0x0400100e = (*(vu16 *)0x0400100e & 0x43) | 0x8410;
        SetBlend_0205aae4();
        if (gGameState.unk0 == 0)
            Sound_PlayVoice(0, 0, 0x22);
        else
            Sound_PlayVoice(0, 0, 0x2b);
    }
    w->unk8ce = 0;
    w->fn474 = func_0205aacc;
}
