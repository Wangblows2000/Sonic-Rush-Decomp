#include <nitro/types.h>

typedef struct { s32 unk0; s32 unk4; s32 unk8; s32 unkC; } UnkWork_02050c88;
typedef struct { u8 pad0[0x10]; UnkWork_02050c88 *work; } UnkTask_02050c88;
typedef struct { u16 unk0; u16 unk2; } UnkStruct_0207ad9c;
typedef struct { u32 pad0[8]; } UnkCfg_02050c88;
extern UnkTask_02050c88 *data_022c4638;
extern s32 data_022c463c;
extern UnkStruct_0207ad9c data_0207ad9c;

void func_02051628(s32 a);
void func_02051838(void);
void func_0206cd7c(s32 a);
UnkTask_02050c88 *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_02050344(void);
void func_02050900(void);
void func_02051248(UnkCfg_02050c88 *c);
void func_020511ec(UnkCfg_02050c88 *c);
void func_02042bbc(UnkCfg_02050c88 *c, s32 a);

#pragma thumb on
void func_02050c88(s32 mode) {
    UnkWork_02050c88 *w;
    UnkCfg_02050c88 cfg;

    func_02051628(0);
    func_02051838();
    func_0206cd7c(0);
    data_022c463c = 0;
    data_022c4638 = func_020372d0(func_02050344, func_02050900, 0, 0x1000, 0, sizeof(UnkWork_02050c88));
    w = data_022c4638->work;
    w->unk0 = 2;
    w->unk4 = 1;
    w->unk8 = 0;
    w->unkC = 0;
    data_0207ad9c.unk0 = w->unk0;
    data_0207ad9c.unk2 = w->unk4;
    if (mode == 0)
        func_02051248(&cfg);
    else
        func_020511ec(&cfg);
    func_02042bbc(&cfg, 0);
}
#pragma thumb reset
