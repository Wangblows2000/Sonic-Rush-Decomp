#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u32 flags8;
} UnkSub3_0205f25c;

typedef struct {
    u8 pad0[8];
    UnkSub3_0205f25c *p8;
} UnkSub2_0205f25c;

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
    u8 pad18[0x44];
    u16 flags5c;
    u8 pad5e[0x12];
    void (*fn70)(void);
    u8 pad74[0x1a4];
    UnkSub2_0205f25c *p218;
    u8 pad21c[0x110];
    u32 flags32c;
} UnkObj_0205f25c;

typedef struct {
    u32 w[5];
} UnkCam_0205f25c;

typedef void (*Fn_0205f25c)(void);

typedef struct {
    Fn_0205f25c fn0;
    UnkObj_0205f25c *obj;
    void (*fn8)(void);
    u8 padc[8];
    UnkObj_0205f25c *obj2;
    void (*fn18)(void);
    u8 pad1c[4];
    Fn_0205f25c fn20;
    u8 pad24[0x54];
    Fn_0205f25c fn78;
    u8 pad7c[0xc];
    Fn_0205f25c fn88;
    u8 pad8c[0x54];
    Fn_0205f25c fne0;
    u8 pade4[0x9c];
    Fn_0205f25c fn180;
    u8 pad184[0x64];
    u32 unk1e8;
    u32 pad1ec;
    Fn_0205f25c fn1f0;
    u8 pad1f4[0x64];
    u32 unk258;
    u16 unk25c;
    u16 pad25e;
    Fn_0205f25c fn260;
    u8 pad264[0xc0];
    Fn_0205f25c fn324;
    u8 pad328[0xc0];
    Fn_0205f25c fn3e8;
    u8 pad3ec[0x18];
    Fn_0205f25c fn404;
    u8 pad408[0x18];
    Fn_0205f25c fn420;
    u8 pad424[0x50];
    Fn_0205f25c fn474;
    Fn_0205f25c fn478;
    u32 unk47c;
    s32 objX;
    s32 objY;
    u32 flags;
    u32 pad48c;
    void *res[4];
    UnkCam_0205f25c cam;
    s32 v4b4;
    s32 v4b8;
    s32 v4bc;
    s32 v4c0;
    s32 v4c4;
    s32 v4c8;
    s32 v4cc;
    s32 v4d0;
    s32 v4d4;
    u16 unk4d8;
} UnkWork_0205f25c;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0205f25c *work;
} UnkTask_0205f25c;

typedef struct {
    s32 unk0;
    u8 pad4[0xc];
    s32 unk10;
    u32 pad14;
    u32 flags;
    u8 zone;
    u8 act;
} UnkState_0205f25c;

typedef struct {
    u8 pad0[0x15];
    u8 b15;
} UnkState2_0205f25c;

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
} UnkGfx_0205f25c;

typedef struct {
    u32 flags;
} UnkG_0205f25c;

extern UnkWork_0205f25c *data_022c4d98;
extern void *gActComArchive;
extern char data_0207c614[];
extern u8 gGameLanguage;
extern u16 data_02076b74[];
extern UnkG_0205f25c gBossStatus;
extern UnkObj_0205f25c *gPlayers[];
extern UnkState_0205f25c gGameState;
extern UnkState2_0205f25c data_022c4590;
extern UnkCam_0205f25c data_ov000_0236a448;
extern UnkGfx_0205f25c gCamera;

void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void StageTitleCard_InitBg(UnkWork_0205f25c *w);
void Hud_SetOamPriority3(void);
void StageTitleCard_DrawPlayer2Hidden(void);
void StageTitleCard_HoldPlayerX(void);
void StageTitleCard_HoldPlayerY(void);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
void StageTitleCard_InitSprites(UnkWork_0205f25c *w);
void StageTitleCard_InitBgBoss(UnkWork_0205f25c *w);
void NNS_G3dGlbLightColor(u32 index, u32 value);
void G3X_SetEdgeColorTable(const u16 *table);
void StageTitleCard_Main(void);
void StageTitleCard_Destructor(void);
void StageTitleCard_State_Init(void);
void StageTitleCard_HoldPlayerNone(void);
void StageTitleCardBg_State_Init(void);
void StageTitleCardStrip_State_Init(void);
void StageTitleCardPartE0_State_Init(void);
void StageTitleCardPart180_State_Init(void);
void StageTitleCardPart260_State_Init(void);
void StageTitleCardPart324_State_Init(void);
void StageTitleCardText3E8_State_Init(void);
void StageTitleCardText404_State_Init(void);
void StageTitleCardPart420_State_Init(void);
void StageTitleCardPlayer_State_Init(void);
void StageTitleCardPart20_State_Init(void);

#define REG_DISPCNT (*(vu32 *)0x04000000)
#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

void StageTitleCard_Create(void) {
    UnkWork_0205f25c *w;
    u32 v;
    BOOL b;
    BOOL b2;
    u32 f[0x18];

    gBossStatus.flags &= ~1;
    w = ((UnkTask_0205f25c *)Task_Create(StageTitleCard_Main, StageTitleCard_Destructor, 0, 0x3efe, 3, 0x8d0))->work;
    data_022c4d98 = w;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, w, 0x8d0);
    }
    w->fn474 = StageTitleCard_State_Init;
    w->fn478 = StageTitleCard_HoldPlayerNone;
    w->fn78 = StageTitleCardBg_State_Init;
    w->fn88 = StageTitleCardStrip_State_Init;
    w->fne0 = StageTitleCardPartE0_State_Init;
    w->fn180 = StageTitleCardPart180_State_Init;
    w->fn1f0 = StageTitleCardPart180_State_Init;
    w->fn260 = StageTitleCardPart260_State_Init;
    w->fn324 = StageTitleCardPart324_State_Init;
    w->fn3e8 = StageTitleCardText3E8_State_Init;
    w->fn404 = StageTitleCardText404_State_Init;
    w->fn420 = StageTitleCardPart420_State_Init;
    w->fn0 = StageTitleCardPlayer_State_Init;
    w->fn20 = StageTitleCardPart20_State_Init;
    w->obj = gPlayers[0];
    w->objX = w->obj->x;
    w->objY = w->obj->y;
    w->fn8 = w->obj->fn70;
    w->obj->flags5c &= ~0x40;
    if (gGameState.unk10 == 1) {
        w->obj2 = gPlayers[1];
        w->fn18 = w->obj2->fn70;
        w->obj2->flags5c &= ~0x40;
        w->obj2->fn70 = StageTitleCard_DrawPlayer2Hidden;
    }
    w->flags |= 0x80025;
    w->unk4d8 = 0x1f;
    w->unk1e8 = 0;
    w->unk258 = 0x10000;
    w->unk25c = 0xf;
    w->unk47c = gGameState.unk0 == 1;
    w->obj->flags32c |= 0x200000;
    w->obj->flags32c |= 0x400000;

    b = FALSE;
    if (gGameState.zone < 9 && gGameState.act < 2)
        b = TRUE;
    if (b) {
        u8 zone = gGameState.zone;
        if (zone == 0 || zone == 1 || (zone == 3 && gGameState.act == 0) || (zone == 4 && gGameState.act == 0)
            || (zone == 5 && gGameState.act == 1) || zone == 6) {
            if ((gGameState.flags & 2) == 0) {
                w->obj->p218->p8->flags8 |= 1;
                w->fn478 = StageTitleCard_HoldPlayerX;
            }
        } else if ((gGameState.flags & 2) == 0 || data_022c4590.b15 == 0) {
            w->obj->p218->p8->flags8 |= 1;
            w->flags |= 0x800000;
            w->fn478 = StageTitleCard_HoldPlayerY;
        }
    }
    w->cam = data_ov000_0236a448;
    w->v4b4 = 0x9e000;
    w->v4b8 = -0x5b000;
    w->v4bc = 0x1e000;
    w->v4c0 = 0x80000;
    w->v4c4 = -0x5b000;
    w->v4c8 = 0;
    w->v4cc = 0;
    w->v4d0 = 0x1000;
    w->v4d4 = 0;
    gCamera.unk0 &= ~3;
    gCamera.unk64 &= ~3;
    if (gGameState.zone == 1)
        gCamera.unkc8 |= 0x2000000;
    gCamera.unkc8 |= 0x800;
    gGameState.flags &= ~1;
    gCamera.unk34 |= 1;
    gCamera.unk98 |= 1;
    if (gGameState.zone >= 9 || gGameState.act >= 2) {
        REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | 0x1500;
        REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | 0x1c00;
    } else {
        *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & 0x43) | 0xa000;
        *(vu16 *)0x0400100c = (*(vu16 *)0x0400100c & 0x43) | 0xa000;
        *(vu16 *)0x0400100e = (*(vu16 *)0x0400100e & 0x43) | 0x4400;
    }
    NNS_FndMountArchive(f, data_0207c614, gActComArchive);
    w->res[0] = NNS_FndGetArchiveFileByIndex(f, 2);
    switch (gGameLanguage) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
        v = gGameLanguage;
        break;
    default:
        v = 1;
        break;
    }
    w->res[1] = NNS_FndGetArchiveFileByIndex(f, data_02076b74[v]);
    b2 = FALSE;
    if (gGameState.zone < 9 && gGameState.act < 2)
        b2 = TRUE;
    if (!b2) {
        w->res[2] = NNS_FndGetArchiveFileByIndex(f, gGameState.zone + 0x10);
        w->res[3] = NNS_FndGetArchiveFileByIndex(f, 0xf);
    } else {
        w->res[2] = NNS_FndGetArchiveFileByIndex(f, gGameState.zone + 6);
        w->res[3] = NNS_FndGetArchiveFileByIndex(f, 5);
    }
    NNS_FndUnmountArchive(f);
    StageTitleCard_InitSprites(w);
    if (gGameState.zone >= 9 || gGameState.act >= 2)
        StageTitleCard_InitBgBoss(w);
    else
        StageTitleCard_InitBg(w);
    NNS_G3dGlbLightColor(0, 0);
    NNS_G3dGlbLightColor(1, 0);
    NNS_G3dGlbLightColor(2, 0);
    {
        u16 edge[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        G3X_SetEdgeColorTable(edge);
    }
    Hud_SetOamPriority3();
}
