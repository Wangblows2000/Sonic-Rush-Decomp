#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    s32 y;
} UnkObj_0205ef48;

typedef struct {
    u16 h0;
    u16 pad2;
    u32 w4;
    u32 w8;
    u32 wc;
    u32 w10;
} UnkCam_0205ef48;

typedef void (*Fn_0205ef48)(void);

typedef struct {
    Fn_0205ef48 fn0;
    UnkObj_0205ef48 *obj;
    u8 pad8[0x18];
    Fn_0205ef48 fn20;
    u8 pad24[0x54];
    Fn_0205ef48 fn78;
    u8 pad7c[0xc];
    Fn_0205ef48 fn88;
    u8 pad8c[0x54];
    Fn_0205ef48 fne0;
    u8 pade4[0x9c];
    Fn_0205ef48 fn180;
    u8 pad184[0x64];
    u32 unk1e8;
    u32 pad1ec;
    Fn_0205ef48 fn1f0;
    u8 pad1f4[0x64];
    u32 unk258;
    u16 unk25c;
    u16 pad25e;
    Fn_0205ef48 fn260;
    u8 pad264[0xc0];
    Fn_0205ef48 fn324;
    u8 pad328[0xc0];
    Fn_0205ef48 fn3e8;
    u8 pad3ec[0x18];
    Fn_0205ef48 fn404;
    u8 pad408[0x18];
    Fn_0205ef48 fn420;
    u8 pad424[0x50];
    Fn_0205ef48 fn474;
    Fn_0205ef48 fn478;
    u32 unk47c;
    s32 objX;
    s32 objY;
    u32 flags;
    u32 pad48c;
    void *res[4];
    u16 camH;
    u16 pad4a2;
    u32 cam4;
    u32 cam8;
    u32 camC;
    u32 cam10;
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
} UnkWork_0205ef48;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0205ef48 *work;
} UnkTask_0205ef48;

extern UnkWork_0205ef48 *data_022c4d98;
extern void *gExtraBossArchive;
extern char data_0207c610[];
extern u8 gGameLanguage;
extern u16 data_02076b5c[];

void *Task_Create(void (*func)(void), void *dtor, u16 prio, u16 id, u16 unk1a, u32 workSize);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
u8 *ExtraBoss_GetWork(void);
UnkCam_0205ef48 *func_ov001_022f71f4(void);
void NNS_FndMountArchive(void *f, void *path, void *arc);
void *NNS_FndGetArchiveFileByIndex(void *f, s32 i);
void NNS_FndUnmountArchive(void *f);
void StageTitleCard_InitSprites(UnkWork_0205ef48 *w);
void StageTitleCard_InitBgBoss(UnkWork_0205ef48 *w);
void NNS_G3dGlbLightColor(u32 index, u32 value);
void G3X_SetEdgeColorTable(const u16 *table);
void StageTitleCard_Main(void);
void StageTitleCard_Destructor(void);
void StageTitleCard_State_Init(void);
void func_0205b560(void);
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

void StageTitleCard_CreateExtraBoss(void) {
    UnkWork_0205ef48 *w;
    UnkCam_0205ef48 *cam;
    u32 v;
    u32 f[0x18];

    w = ((UnkTask_0205ef48 *)Task_Create(StageTitleCard_Main, StageTitleCard_Destructor, 0, 0x3efe, 3, 0x8d0))->work;
    data_022c4d98 = w;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, w, 0x8d0);
    }
    w->fn474 = StageTitleCard_State_Init;
    w->fn478 = func_0205b560;
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
    w->obj = (UnkObj_0205ef48 *)(ExtraBoss_GetWork() + 0x64);
    w->objX = w->obj->x;
    w->objY = w->obj->y;
    w->flags |= 0x80025;
    w->unk4d8 = 0x1f;
    w->unk1e8 = 0;
    w->unk258 = 0x10000;
    w->unk25c = 0xf;
    w->unk47c = 0;
    cam = func_ov001_022f71f4();
    w->camH = cam->h0;
    w->cam4 = cam->w4;
    w->cam8 = cam->w8;
    w->camC = cam->wc;
    w->cam10 = cam->w10;
    w->v4b4 = 0x9e000;
    w->v4b8 = -0x5b000;
    w->v4bc = 0x1e000;
    w->v4c0 = 0x80000;
    w->v4c4 = -0x5b000;
    w->v4c8 = 0;
    w->v4cc = 0;
    w->v4d0 = 0x1000;
    w->v4d4 = 0;
    REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | 0x1500;
    REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | 0x1c00;
    NNS_FndMountArchive(f, data_0207c610, gExtraBossArchive);
    w->res[0] = NNS_FndGetArchiveFileByIndex(f, 0);
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
    w->res[1] = NNS_FndGetArchiveFileByIndex(f, data_02076b5c[v]);
    w->res[2] = NNS_FndGetArchiveFileByIndex(f, 4);
    w->res[3] = NNS_FndGetArchiveFileByIndex(f, 3);
    NNS_FndUnmountArchive(f);
    StageTitleCard_InitSprites(w);
    StageTitleCard_InitBgBoss(w);
    NNS_G3dGlbLightColor(0, 0);
    NNS_G3dGlbLightColor(1, 0);
    NNS_G3dGlbLightColor(2, 0);
    {
        u16 edge[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        G3X_SetEdgeColorTable(edge);
    }
}
