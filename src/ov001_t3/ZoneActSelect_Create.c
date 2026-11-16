#include <nitro/types.h>

typedef struct UnkTask_022e190c UnkTask_022e190c;

typedef struct {
    s32 unk0;
    u8 pad4[8];
    s32 unkc;
    s32 unk10;
    u8 pad14[0xd8 - 0x14];
    s32 unkd8;
    s32 unkdc;
    u8 pade0[0xea8 - 0xe0];
    UnkTask_022e190c *task;
    u8 padeac[0xec0 - 0xeac];
} UnkWork_022e190c;

struct UnkTask_022e190c {
    u8 pad0[0x10];
    UnkWork_022e190c *work;
};

extern UnkWork_022e190c *gZoneActSelectWork;
extern s32 data_ov001_0231c3ec;
extern s32 gZoneActSelectResult;

void ZoneActSelect_IsActive(void);
UnkTask_022e190c *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void ZoneActSelect_State_FadeIn(void);
void ZoneActSelect_Destructor(void);
void ZoneActSelect_LoadRecords(UnkWork_022e190c *w);
void ZoneActSelect_Unk_ov001_022e06e4(UnkWork_022e190c *w);
void ZoneActSelect_InitDisplay(void);
void ZoneActSelect_LoadAssets(UnkWork_022e190c *w);
void ZoneActSelect_Unk_ov001_022e153c(UnkWork_022e190c *w);
void ZoneActSelect_ShowIntroNaviMessage(void);

#pragma thumb on
void ZoneActSelect_Create(s32 a, s32 b, s32 c) {
    vu32 zero;
    UnkTask_022e190c *t;
    UnkWork_022e190c *w;

    ZoneActSelect_IsActive();
    t = Task_Create(ZoneActSelect_State_FadeIn, ZoneActSelect_Destructor, 0, 0x8000, 0, sizeof(UnkWork_022e190c));
    w = t->work;
    gZoneActSelectWork = w;
    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkWork_022e190c));
    gZoneActSelectWork->unk0 = 0;
    gZoneActSelectWork->unkc = b + 1;
    gZoneActSelectWork->task = t;
    gZoneActSelectWork->unkd8 = 0;
    gZoneActSelectWork->unkdc = 0;
    gZoneActSelectWork->unk10 = c;
    data_ov001_0231c3ec = a;
    gZoneActSelectResult = 0;
    ZoneActSelect_LoadRecords(gZoneActSelectWork);
    ZoneActSelect_Unk_ov001_022e06e4(gZoneActSelectWork);
    ZoneActSelect_InitDisplay();
    ZoneActSelect_LoadAssets(gZoneActSelectWork);
    ZoneActSelect_Unk_ov001_022e153c(gZoneActSelectWork);
    ZoneActSelect_ShowIntroNaviMessage();
}
#pragma thumb reset
