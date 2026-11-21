#include <game/Stage/stageobj_alt.h>

typedef struct {
    s32 unk0;
    u16 unk4;
    u8 pad6[2];
} UnkWork_0232ecac;

void TensionSound_Main(void);
void TensionSound_Destructor(void);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);

void TensionSound_Create(s32 a, u16 b) {
    UnkStruct_Task *task;
    UnkWork_0232ecac *w;
    vu16 zero;

    task = Task_Create(TensionSound_Main, TensionSound_Destructor, 0, 0xa00, 4, sizeof(UnkWork_0232ecac));
    if (task == OS_GetArenaLo(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_0232ecac));
    w->unk0 = Sound_AllocHandle();
    Sound_PlaySeqArcEx(w->unk0, 0, a);
    w->unk4 = b;
}
