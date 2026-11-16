#include <ov000_b/stageobj.h>

typedef struct {
    s32 unk0;
    u16 unk4;
    u8 pad6[2];
} UnkWork_0232ecac;

void func_ov000_0232ec1c(void);
void func_ov000_0232ec74(void);
void Sound_PlaySeqArcEx(s32 a, s32 b, s32 c);

void func_ov000_0232ecac(s32 a, u16 b) {
    UnkStruct_Task *task;
    UnkWork_0232ecac *w;
    vu16 zero;

    task = Task_Create(func_ov000_0232ec1c, func_ov000_0232ec74, 0, 0xa00, 4, sizeof(UnkWork_0232ecac));
    if (task == func_02006abc(0))
        return;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_0232ecac));
    w->unk0 = Sound_AllocHandle();
    Sound_PlaySeqArcEx(w->unk0, 0, a);
    w->unk4 = b;
}
