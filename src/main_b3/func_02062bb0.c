#include <nitro/types.h>

typedef struct UnkWork_02062bb0 {
    void (*fn)(struct UnkWork_02062bb0 *w);
    u8 pad4[4];
    s32 unk8;
} UnkWork_02062bb0;
typedef struct { u8 pad0[0x10]; UnkWork_02062bb0 *work; } UnkTask_02062bb0;
extern UnkTask_02062bb0 *gTaskCurrent;

BOOL TalkDemo_IsActive(void);
void Task_Destroy(UnkTask_02062bb0 *t);
u32 TalkDemo_GetFlags(s32 a);
void func_02061a78(s32 a);
void func_02061870(void *p);
void func_02061130(UnkWork_02062bb0 *w);

#pragma thumb on
void func_02062bb0(void) {
    UnkWork_02062bb0 *w = gTaskCurrent->work;

    if (!TalkDemo_IsActive()) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    if (TalkDemo_GetFlags(3) & 0x40000000) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    w->unk8++;
    if (TalkDemo_GetFlags(3) & 0x20000000) {
        func_02061a78(0);
        func_02061870(NULL);
        w->fn = NULL;
    }
    if (w->fn != NULL)
        w->fn(w);
    func_02061130(w);
    if (TalkDemo_GetFlags(3) & 0x80000000)
        Task_Destroy(gTaskCurrent);
}
#pragma thumb reset
