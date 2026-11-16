#include <nitro/types.h>

typedef struct { u8 data[0x28]; } UnkWin_0205fc00;
typedef struct {
    u8 pad0[0x16];
    u8 unk16;
    u8 unk17;
    u8 pad18[0x34];
    UnkWin_0205fc00 win[4]; /* 0x4c */
} UnkWork_0205fc00;
extern UnkWork_0205fc00 *data_022c4d9c;
extern void *gTaskCurrent;

void Task_Destroy(void *t);
u32 TalkDemo_GetFlags(s32 a);
void func_0205fad0(void);
void func_0205fb30(void);
void OS_CancelVAlarm(UnkWin_0205fc00 *p);
void OS_SetVAlarm(UnkWin_0205fc00 *p, s32 a, s32 b, void (*cb)(void), s32 i);

#pragma thumb on
void func_0205fc00(void) {
    UnkWork_0205fc00 *w = data_022c4d9c;
    void (*cb)(void);
    s16 lim[4];
    u8 i;
    UnkWin_0205fc00 *p;

    if (w == NULL) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    if (TalkDemo_GetFlags(0) & 2)
        cb = func_0205fad0;
    else
        cb = func_0205fb30;
    lim[0] = w->unk16 - 1;
    lim[1] = w->unk16 + 2;
    lim[2] = w->unk17 - 4;
    lim[3] = w->unk17 - 1;
    for (i = 0; i < 4; i++) {
        p = &w->win[i];
        OS_CancelVAlarm(p);
        if (i == 1 && lim[1] >= lim[2])
            i = 2;
        else
            OS_SetVAlarm(p, lim[i], 5, cb, i);
    }
}
#pragma thumb reset
