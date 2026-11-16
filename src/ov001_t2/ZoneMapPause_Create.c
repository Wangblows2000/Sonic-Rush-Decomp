#include <nitro/types.h>

typedef struct UnkTask_022e95b8 UnkTask_022e95b8;

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padc[0x190 - 0xc];
    UnkTask_022e95b8 *task;
} UnkWork_022e95b8;

struct UnkTask_022e95b8 {
    u8 pad0[0x10];
    UnkWork_022e95b8 *work;
};

extern UnkWork_022e95b8 *gZoneMapPauseWork;

void ZoneMapPause_Destroy(void);
void ZoneMapPause_InitDisplay(void);
UnkTask_022e95b8 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void ZoneMapPause_LoadAssets(UnkWork_022e95b8 *w);
void ZoneMapPause_Unk_ov001_022e926c(UnkWork_022e95b8 *w);
void ZoneMapPause_Unk_ov001_022e8d44(void);
void ZoneMapPause_State_Open(void);
void ZoneMapPause_Destructor(void);

#pragma thumb on
static inline u32 GetPlane_022e95b8(void) {
    return (*(vu32 *)0x4000000 & 0x1f00) >> 8;
}

static inline void SetPlane_022e95b8(u32 plane) {
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | (plane << 8);
}

void ZoneMapPause_Create(BOOL flag) {
    UnkTask_022e95b8 *t;
    UnkWork_022e95b8 *w;

    ZoneMapPause_Destroy();
    ZoneMapPause_InitDisplay();
    t = Task_Create(ZoneMapPause_State_Open, ZoneMapPause_Destructor, 0, 0x8000, 0x1000, 0x194);
    w = t->work;
    gZoneMapPauseWork = w;
    {
        vu32 z = 0;
        MIi_CpuClear32(z, w, 0x194);
    }
    gZoneMapPauseWork->unk0 = 0;
    gZoneMapPauseWork->unk4 = 0;
    gZoneMapPauseWork->unk8 = 0;
    gZoneMapPauseWork->task = t;
    ZoneMapPause_LoadAssets(gZoneMapPauseWork);
    ZoneMapPause_Unk_ov001_022e926c(gZoneMapPauseWork);
    SetPlane_022e95b8(GetPlane_022e95b8() | 4);
    if (flag) {
        ZoneMapPause_Unk_ov001_022e8d44();
    }
}
#pragma thumb reset
