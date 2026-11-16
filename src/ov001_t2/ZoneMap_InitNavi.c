#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkc;
    s32 unk10;
    s32 mode;
} UnkWork_022e33c0;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022e33c0 *work;
} UnkTask_022e33c0;

void NaviBg_Load(s32 a, s32 b);
void NaviM_Load(void);
void NaviMsg_Create(void);
void NaviM_Unk_0206488c(void);
UnkTask_022e33c0 *Task_Create(void (*fn)(void), s32 a, s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void NaviMsg_SetVisible(s32 a);
void ZoneMap_DelayedCall_Main(void);

#pragma thumb on
UnkWork_022e33c0 *ZoneMap_InitNavi(s32 mode, s32 keep) {
    UnkWork_022e33c0 *w;

    switch (mode) {
    case 0:
        NaviBg_Load(0, 0);
        break;
    case 1:
        NaviBg_Load(0, 1);
        break;
    }
    NaviM_Load();
    NaviMsg_Create();
    if (keep == 0) {
        NaviM_Unk_0206488c();
    }
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1400;
    w = Task_Create(ZoneMap_DelayedCall_Main, 0, 0, 0x3000, 0x1000, sizeof(UnkWork_022e33c0))->work;
    {
        vu32 z = 0;
        MIi_CpuClear32(z, w, sizeof(UnkWork_022e33c0));
    }
    w->unk0 = 0x20;
    w->unk8 = 0;
    w->unkc = 0;
    w->mode = mode;
    w->unk4 = 0;
    NaviMsg_SetVisible(0);
    return w;
}
#pragma thumb reset
