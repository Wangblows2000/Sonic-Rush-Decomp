#include <nitro/types.h>

typedef struct UnkWork_02060d00 {
    void (*fn)(struct UnkWork_02060d00 *w);
    u8 pad4[0x1c];
    s32 unk20;
    void *unk24;
} UnkWork_02060d00;
typedef struct { u8 pad0[4]; s32 unk4; } UnkInfo_02060d00;
typedef struct { u8 pad0[8]; s16 unk8; } UnkBlend_02060d00;
extern s16 gTalkDemoId;
extern void *gTaskCurrent;
extern UnkBlend_02060d00 data_0208926c;

void *TalkDemo_LoadScript(s32 a, u8 b);
void Task_Destroy(void *t);
UnkInfo_02060d00 *func_02060470(void *p, s32 a);
void TalkDemo_SetFlags(s32 a, s32 b);
void func_02062c9c(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void func_020602dc(void);
u32 TalkDemo_GetFlags(s32 a);
void func_02060288(void);
void func_0206086c(UnkWork_02060d00 *w);
void func_020607d4(UnkWork_02060d00 *w);
void func_02060ce0(UnkWork_02060d00 *w);

#pragma thumb on
void func_02060d00(UnkWork_02060d00 *w) {
    vu32 zero;

    w->unk24 = TalkDemo_LoadScript(w->unk20, gTalkDemoId);
    if (w->unk24 == NULL) {
        Task_Destroy(gTaskCurrent);
        return;
    }
    if (func_02060470(w->unk24, 0)->unk4 == 3) {
        TalkDemo_SetFlags(0, 2);
        data_0208926c.unk8 = 0;
    }
    func_02062c9c();
    zero = 0;
    MIi_CpuClear32(zero, (void *)0x06002000, 0xa000);
    func_020602dc();
    if (!(TalkDemo_GetFlags(0) & 2))
        func_02060288();
    func_0206086c(w);
    func_020607d4(w);
    w->fn = func_02060ce0;
}
#pragma thumb reset
