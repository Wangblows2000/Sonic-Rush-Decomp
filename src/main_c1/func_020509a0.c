#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 pad8[4];
    s32 unkC;
    u8 pad10[0xe];
    u16 name[10];  /* 0x1e */
    u16 unk32;
    u16 unk34;
    u16 unk36;
} UnkWork_020509a0;
typedef struct { u8 pad0[0x10]; UnkWork_020509a0 *work; } UnkTask_020509a0;
typedef struct { u16 unk0; u16 unk2; } UnkStruct_0207ad9c;
typedef struct { u8 pad0[0x18]; u16 unk18; } UnkStruct_02072cb8;
typedef struct { u32 pad0[6]; u8 unk18; u8 pad19[7]; } UnkCfg_020509a0;
extern UnkTask_020509a0 *data_022c4638;
extern UnkStruct_0207ad9c data_0207ad9c;
extern u16 data_02072cbc[];
extern UnkStruct_02072cb8 data_02072cb8;
extern s32 data_022c4640;

void func_020371c8(UnkTask_020509a0 *t);
UnkTask_020509a0 *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_02050510(void);
void func_02050900(void);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void func_02051628(s32 a);
void func_020512a4(UnkCfg_020509a0 *c);
void func_02051248(UnkCfg_020509a0 *c);
void func_02042bbc(UnkCfg_020509a0 *c, s32 a);

#pragma thumb on
UnkWork_020509a0 *func_020509a0(s32 mode) {
    UnkWork_020509a0 *w;
    vu16 zero;
    s32 i;
    u16 n;
    UnkCfg_020509a0 cfg;

    if (data_022c4638 != NULL) {
        func_020371c8(data_022c4638);
        data_022c4638 = NULL;
    }
    data_022c4638 = func_020372d0(func_02050510, func_02050900, 0, 0x1000, 0, sizeof(UnkWork_020509a0));
    w = data_022c4638->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_020509a0));
    w->unk0 = 2;
    w->unk4 = 1;
    data_0207ad9c.unk0 = w->unk0;
    data_0207ad9c.unk2 = w->unk4;
    MIi_CpuCopy16(data_02072cbc, w->name, 0x14);
    n = data_02072cb8.unk18;
    for (i = n; i < 10; i++)
        w->name[i] = 0x20;
    w->unk32 = i = n;
    func_02051628(0);
    switch (mode) {
    case 0:
        func_020512a4(&cfg);
        break;
    case 1:
    default:
        func_02051248(&cfg);
        break;
    }
    func_02042bbc(&cfg, 0);
    w->unk34 = cfg.unk18 - 1;
    w->unk36 = 0;
    w->unkC = 1;
    data_022c4640 = 0;
    return w;
}
#pragma thumb reset
