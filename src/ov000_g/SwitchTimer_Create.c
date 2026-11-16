#include <ov000_b/stageobj.h>

typedef struct {
    s16 unk0;
    u16 unk2;
    s16 unk4;
    u8 pad6[2];
    void *unk8;
} UnkStruct_02318ba0;

typedef struct {
    u8 pad0[0x330];
    u32 unk330;
} UnkStruct_02318ba0_P;

extern s16 gSwitchTimers[];
extern UnkStruct_02318ba0_P *gPlayers;

void SwitchTimer_Main(void);
void SwitchTimer_Destructor(void);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void func_ov000_0233e48c(s32 a);

void SwitchTimer_Create(s16 a, u16 b) {
    UnkStruct_Task *task;
    UnkStruct_02318ba0 *w;
    vu16 zero;

    task = Task_Create((void (*)(void))SwitchTimer_Main, SwitchTimer_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_02318ba0));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02318ba0));
    w->unk0 = a;
    w->unk2 = b;
    Sound_PlaySeqArcEx(NULL, 0, 0x6e);
    w->unk8 = (void *)Sound_AllocHandle();
    if (gSwitchTimers[w->unk0] != 0x3fc)
        Sound_PlaySeqArcEx(w->unk8, 0, 0x6f);
    w->unk4 = 0;
    if (w->unk2 == 0)
        return;
    if (!(gPlayers->unk330 & 0x8000))
        return;
    func_ov000_0233e48c(3);
    gPlayers->unk330 |= 0x4000;
}
