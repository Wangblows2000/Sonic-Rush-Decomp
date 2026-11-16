#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    u8 pad4[0xc];
} UnkSub_020480ec;

typedef struct {
    void (*fn0)(void);
    u8 pad4[0xc];
    void (*fn10)(void);
    u8 pad14[0x158];
    void (*fn16c)(void);
    u8 pad170[0x90];
    void (*fn200)(void);
    u8 pad204[0x70];
    void (*fn274)(void);
    u8 pad278[0xb8];
    UnkSub_020480ec sub330;
    void (*fn340)(void);
    u8 pad344[4];
    void *res348;
    void *res34c;
    s16 unk350;
    u8 pad352[2];
    s32 unk354;
    s32 unk358;
    s32 unk35c;
    s32 unk360;
    s32 unk364;
    s32 unk368;
    s32 unk36c;
    s32 unk370;
    s32 unk374;
    s32 unk378;
    s32 unk37c;
    s32 unk380;
    s32 unk384;
    u32 unk388;
    u32 unk38c;
    u32 unk390;
    void *res394;
    void *unk398;
    u8 pad39c[0x1630 - 0x39c];
    s32 unk1630;
    u8 unk1634[4];
} UnkWork_020480ec;

typedef struct { u8 pad0[0x10]; UnkWork_020480ec *work; } UnkTask_020480ec;

typedef struct {
    u32 unk0;
    u8 pad4[0xc];
    s32 unk10;
    u8 pad14[8];
    u8 unk1c;
    u8 unk1d;
    u8 pad1e[2];
    u32 unk20;
} UnkStruct_022c4560;

typedef struct { u8 pad0[8]; s16 unk8; } UnkBright_020480ec;
typedef struct { u8 pad0[4]; u16 unk4; u16 unk6; } UnkStruct_0208922c;
typedef struct { u8 pad0[8]; u16 unk8; u16 unka; } UnkStruct_02369ac0;
typedef struct { u32 data[0x17]; } UnkArc_020480ec;

extern UnkStruct_022c4560 gGameState;
extern UnkBright_020480ec data_0208926c;
extern UnkBright_020480ec data_02087d44;
extern UnkStruct_0208922c gDisplayMain;
extern UnkStruct_02369ac0 gBossStatus;
extern u32 data_ov000_0236ac50[][2];
extern u32 data_ov000_0236ac40;
extern u32 data_ov000_0236ac48;
extern char data_0207923c[];
extern char data_02079240[];
extern char data_02079258[];
extern void *gActComArchive;
extern void *gSoundHeap;

void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);
void NNS_SndStopSoundAll(void);
UnkTask_020480ec *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void ClearDemo_Main(void);
void ClearDemo_Destructor(void);
void ClearDemo_State_FadeIn(void);
void ClearDemoBg_State_Init(void);
void ClearDemoName_State_Init(void);
void ClearDemoModel_State_Init(void);
void ClearDemoActText_State_Init(void);
void ClearDemoBars_State_SlideIn(void);
void ClearDemoTA_State_SlideIn(void);
void ClearDemoScore_State_SlideIn(void);
void ClearDemoBars_Init(UnkSub_020480ec *s);
void ClearDemo_InitTimeAttackTally(void *p);
void ClearDemo_InitScoreTally(void *p);
void SaveData_InsertTimeRecord(u8 a, u8 b, u16 c, u16 d);
void StoryProgress_Update(void);
void NNS_SndHandleInit(void *p);
void NNS_G3dGlbLightColor(s32 i, u16 v);
void NNS_G3dGlbLightVector(s32 i, s32 x, s32 y, s32 z);
void GX_SetGraphicsMode(s32 a, s32 b, s32 c);
void Vram_SetBankForBGExtPltt(s32 a);
void Vram_SetBankForSubBGExtPltt(s32 a);
void G3X_SetClearColor(u32 a, u32 b, u16 c, u32 d, BOOL e);
BOOL NNS_FndMountArchive(UnkArc_020480ec *arc, const char *name, void *narc);
void *NNS_FndGetArchiveFileByIndex(UnkArc_020480ec *arc, s32 idx);
BOOL NNS_FndUnmountArchive(UnkArc_020480ec *arc);
void *File_Load(void *a, s32 b);
void ClearDemo_InitSprites(UnkWork_020480ec *w);
void ClearDemoTA_InitSprites(UnkWork_020480ec *w);
void ClearDemo_InitModel(UnkWork_020480ec *w);
void ClearDemo_InitBgBoss(UnkWork_020480ec *w);
void ClearDemo_InitBg(UnkWork_020480ec *w);
void NaviMsg_Create(void);
void NaviMsg_SetVisible(s32 a);
void NaviB_Create(void);
void Sound_ResetHeap(void);
void Sound_LoadSoundArchive(void *p);
void NNS_SndArcLoadGroup(s32 a, void *b);
void Sound_PlaySeq(s32 a, s32 b);

#pragma thumb on
void ClearDemo_Create(void) {
    UnkWork_020480ec *w;
    s32 a;
    s32 b;
    vu16 zero;
    UnkArc_020480ec arc;

    GXx_SetMasterBrightness_((vu16 *)0x400006c, -16);
    data_0208926c.unk8 = -16;
    GXx_SetMasterBrightness_((vu16 *)0x400106c, -16);
    data_02087d44.unk8 = -16;
    NNS_SndStopSoundAll();
    w = Task_Create(ClearDemo_Main, ClearDemo_Destructor, 0, 0x3efe, 3, sizeof(UnkWork_020480ec))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_020480ec));
    w->fn340 = ClearDemo_State_FadeIn;
    w->fn0 = ClearDemoBg_State_Init;
    w->fn16c = ClearDemoName_State_Init;
    w->fn10 = ClearDemoModel_State_Init;
    w->fn200 = ClearDemoActText_State_Init;
    w->sub330.fn = ClearDemoBars_State_SlideIn;
    ClearDemoBars_Init(&w->sub330);
    if (gGameState.unk10 == 2) {
        ClearDemo_InitTimeAttackTally(&w->fn274);
        w->fn274 = ClearDemoTA_State_SlideIn;
        SaveData_InsertTimeRecord(gGameState.unk1c, gGameState.unk1d, gGameState.unk20, gGameState.unk0);
    } else {
        ClearDemo_InitScoreTally(&w->fn274);
        w->fn274 = ClearDemoScore_State_SlideIn;
        StoryProgress_Update();
    }
    NNS_SndHandleInit(w->unk1634);
    w->unk350 = 0x1555;
    w->unk354 = 0x1000;
    w->unk358 = 0x800000;
    w->unk35c = 0x1555;
    w->unk364 = 0x80000;
    w->unk368 = -0x86666;
    w->unk36c = 0;
    w->unk370 = 0x80000;
    w->unk374 = -0x60000;
    w->unk378 = 0;
    w->unk37c = 0;
    w->unk380 = 0x1000;
    w->unk384 = 0;
    NNS_G3dGlbLightColor(0, 0x6f7b);
    NNS_G3dGlbLightVector(0, 0, -0x4a0, -0xf40);
    NNS_G3dGlbLightColor(1, 0x2108);
    NNS_G3dGlbLightVector(1, 0xec7, -0x600, 0);
    NNS_G3dGlbLightColor(2, 0x2108);
    NNS_G3dGlbLightVector(2, 0, 0x620, 0xec7);
    if (gGameState.unk1c >= 9 || gGameState.unk1d >= 2) {
        GX_SetGraphicsMode(1, 5, 1);
        Vram_SetBankForBGExtPltt(0);
        Vram_SetBankForSubBGExtPltt(0);
        G3X_SetClearColor(0, 0, 0x7fff, 0, 0);
        *(vu16 *)0x4000008 = (*(vu16 *)0x4000008 & ~3) | 1;
        *(vu16 *)0x400000a = (*(vu16 *)0x400000a & ~3) | 3;
        *(vu16 *)0x400000c = (*(vu16 *)0x400000c & ~3) | 0;
        *(vu16 *)0x400000e = (*(vu16 *)0x400000e & ~3) | 2;
    } else {
        *(vu16 *)0x400100a = (*(vu16 *)0x400100a & 0x43) | 0xe808;
    }
    *(vu16 *)0x400000a = (*(vu16 *)0x400000a & 0x43) | 0x2888;
    gDisplayMain.unk4 = 0;
    gDisplayMain.unk6 = 0;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x4000;
    *(vu16 *)0x400000e = (*(vu16 *)0x400000e & 0x43) | 0x4400;
    w->unk388 = data_ov000_0236ac50[gGameState.unk0][0];
    w->unk38c = data_ov000_0236ac40;
    w->unk390 = data_ov000_0236ac48;
    NNS_FndMountArchive(&arc, data_0207923c, gActComArchive);
    w->res348 = NNS_FndGetArchiveFileByIndex(&arc, 0x1a);
    w->res34c = NNS_FndGetArchiveFileByIndex(&arc, 0x1b);
    w->res394 = NNS_FndGetArchiveFileByIndex(&arc, 0x19);
    NNS_FndUnmountArchive(&arc);
    if (gGameState.unk10 == 2)
        w->unk398 = File_Load(data_02079240, 0);
    ClearDemo_InitSprites(w);
    if (gGameState.unk10 == 2)
        ClearDemoTA_InitSprites(w);
    ClearDemo_InitModel(w);
    if (gGameState.unk1c >= 9 || gGameState.unk1d >= 2)
        ClearDemo_InitBgBoss(w);
    else
        ClearDemo_InitBg(w);
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1f00;
    if (gGameState.unk1c >= 9 || gGameState.unk1d >= 2) {
        NaviMsg_Create();
        NaviMsg_SetVisible(1);
        gBossStatus.unk8 = 0;
        gBossStatus.unka = 8;
        NaviB_Create();
    }
    Sound_ResetHeap();
    Sound_LoadSoundArchive(data_02079258);
    if (gGameState.unk10 == 2) {
        if (gGameState.unk1c >= 9 || gGameState.unk1d >= 2) {
            a = 6;
            w->unk1630 = 5;
            b = 0xd;
        } else {
            a = 5;
            w->unk1630 = 5;
            b = 0xb;
        }
    } else if (gGameState.unk1c == 7) {
        a = 9;
        w->unk1630 = 7;
        b = 0x11;
    } else if (gGameState.unk1c >= 9 || gGameState.unk1d >= 2) {
        a = 8;
        w->unk1630 = 6;
        b = 0x10;
    } else {
        a = 7;
        w->unk1630 = 6;
        b = 0xf;
    }
    NNS_SndArcLoadGroup(a, gSoundHeap);
    Sound_PlaySeq(0, b);
}
#pragma thumb reset
