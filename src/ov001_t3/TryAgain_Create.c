#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    u8 padc[0x5f4 - 0xc];
    s32 unk5f4;
    s32 unk5f8;
    s32 unk5fc;
    s32 unk600;
    void (*fn604)(void);
    void (*fn608)(void);
    void (*fn60c)(void);
    void (*fn610)(void);
} UnkWork_022d40a4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d40a4 *work;
} UnkTask_022d40a4;

UnkTask_022d40a4 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void TryAgain_State_Enter(void);
void TryAgain_Destructor(void);
BOOL func_ov001_022d344c(void);
void TryAgain_ExitToRetry(void);
void TryAgain_ExitToPlayerSelect(void);
void TryAgain_ExitToTimeAttackMenuMode0(void);
void TryAgain_ExitToTimeAttackMenuMode1(void);
void TryAgain_ExitToTitleScreen(void);

#pragma thumb on
UnkWork_022d40a4 *TryAgain_Create(void) {
    UnkWork_022d40a4 *w;
    vu16 zero;

    w = Task_Create(TryAgain_State_Enter, TryAgain_Destructor, 0, 0x1000, 0x1000, sizeof(UnkWork_022d40a4))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_022d40a4));
    w->unk0 = 0;
    w->unk4 = 0;
    w->unk8 = 0;
    w->unk5f4 = 1;
    w->unk5f8 = 4;
    if (func_ov001_022d344c()) {
        w->unk5fc = 7;
    } else {
        w->unk5fc = 10;
    }
    w->unk600 = 0xd;
    w->fn604 = TryAgain_ExitToRetry;
    w->fn608 = TryAgain_ExitToPlayerSelect;
    if (func_ov001_022d344c()) {
        w->fn60c = TryAgain_ExitToTimeAttackMenuMode0;
    } else {
        w->fn60c = TryAgain_ExitToTimeAttackMenuMode1;
    }
    w->fn610 = TryAgain_ExitToTitleScreen;
    return w;
}
#pragma thumb reset
