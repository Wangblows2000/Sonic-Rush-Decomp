#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    void (*sub)(void);
    u8 pad8[8];
    u16 timer;
    u8 pad12[2];
    s32 mode;
    s32 arg;
} UnkWork_0205a148;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0205a148 *work;
} UnkTask_0205a148;

extern u32 gSystemFlags;

UnkTask_0205a148 *Task_Create(void (*fn)(void), void (*fn2)(void), s32 a, s32 b, s32 c, s32 size);
u32 OS_GetArenaLo(s32 i);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void SaveMsg_Main(void);
void SaveMsg_Destructor(void);
void SaveMsg_OnCommitDone(void);
void SaveMsg_OnFormatDone(void);
void SaveMsg_State_Init(void);

void SaveMsg_Create(s32 mode, s32 arg) {
    UnkTask_0205a148 *t;
    UnkWork_0205a148 *w;
    vu16 z;

    t = Task_Create(SaveMsg_Main, SaveMsg_Destructor, 0x20, 0, 3, 0x1c);
    if (t == (UnkTask_0205a148 *)OS_GetArenaLo(0))
        return;
    w = t->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x1c);
    w->mode = mode;
    w->arg = arg;
    switch (mode) {
    case 0:
        w->timer = 0x3c;
        w->sub = SaveMsg_OnCommitDone;
        break;
    case 3:
        w->timer = 0x3c;
        w->sub = SaveMsg_OnFormatDone;
        break;
    case 1:
    case 2:
    default:
        gSystemFlags |= 0x10;
        break;
    }
    w->fn = SaveMsg_State_Init;
}
