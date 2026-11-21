#include <nitro/types.h>

typedef struct {
    u8 pad0[0x22];
    u16 delay[12];
    u16 rate[12];
} UnkPad_022e8a24;

typedef struct {
    u8 pad0[8];
    u32 flags;
} UnkStruct_022c4680;

typedef struct {
    u8 pad0[0x1cc4];
    s32 unk1cc4;
} UnkWork_022e8a24;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
} UnkRes_022e8a24;

extern UnkPad_022e8a24 gPad;
extern s32 gGameState;
extern UnkStruct_022c4680 gSaveData;
extern s32 gSaveDataBusy;
extern u32 gSystemFlags;

void StoryProgress_RestoreFromSave(void);
UnkWork_022e8a24 *ZoneMap_Create(void);
void ZoneMap_LoadAssets(void);
void ZoneMap_InitDisplay(UnkWork_022e8a24 *w);
void ZoneMap_InitWork(UnkWork_022e8a24 *w);
void SysSound_Init(void);
s32 ZoneMap_IsBlaze(void);
s32 ZoneMap_InitNavi(s32 a, s32 b);
UnkRes_022e8a24 *ZoneMap_Unk_ov001_022e8090(UnkWork_022e8a24 *w, s32 a, s32 b);
void NaviM_Create(void);
void Touch_Enable(void);
void SysSound_PlayBGM(s32 a);

#pragma thumb on
void ZoneMap_Start(s32 a, s32 b) {
    UnkWork_022e8a24 *w;
    UnkRes_022e8a24 *r;
    s32 i;
    BOOL flag;

    StoryProgress_RestoreFromSave();
    w = ZoneMap_Create();
    ZoneMap_LoadAssets();
    ZoneMap_InitDisplay(w);
    ZoneMap_InitWork(w);
    SysSound_Init();
    w->unk1cc4 = ZoneMap_InitNavi(ZoneMap_IsBlaze(), a);
    r = ZoneMap_Unk_ov001_022e8090(w, a, b);
    NaviM_Create();
    for (i = 0; i < 12; i++) {
        gPad.delay[i] = 0x14;
        gPad.rate[i] = 1;
    }
    Touch_Enable();
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1900;
    flag = FALSE;
    if (gGameState == 0) {
        if (r->unk4 >= 0x1f) flag = TRUE;
    } else {
        if (r->unk8 >= 0x1e) flag = TRUE;
    }
    if (gSaveData.flags & 0x1000) flag = FALSE;
    if (flag) {
        SysSound_PlayBGM(5);
    } else {
        SysSound_PlayBGM(4);
    }
    gSaveDataBusy = 0;
    gSystemFlags &= ~0x10;
}
#pragma thumb reset
