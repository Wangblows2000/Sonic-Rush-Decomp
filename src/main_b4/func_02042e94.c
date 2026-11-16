#include <nitro/types.h>

typedef struct {
    void (*fn)(void);
    s32 unk4;
    s32 unk8;
    u8 padc[4];
    u16 unk10;
    u16 unk12;
    u16 mask;
    u8 unk16;
    u8 pad17[5];
    void *unk1c;
    u32 unk20;
    u8 pad24[8];
    u8 mac[6];
    u8 pad32[0x2c];
    u16 unk5e;
    u8 pad60[0x192a - 0x60];
    u16 unk192a;
    u8 pad192c[0x1aee - 0x192c];
    u16 unk1aee;
    u16 unk1af0;
    u16 unk1af2;
    u8 pad1af4[0x1cf4 - 0x1af4];
} UnkWork_02042e94;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02042e94 *work;
} UnkTask_02042e94;

extern UnkTask_02042e94 *data_022c4364;
extern u8 data_022c4380[];
extern u32 gSystemFlags;
extern void (*data_0224ff40)(void);

UnkTask_02042e94 *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_02042684(void);
void func_02042670(void);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void func_020415a8(void);
void func_0203e980(u16 a);
void WH_Initialize(void);
void WH_SetGgid(s32 a);
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void func_02041300(void);

void func_02042e94(u16 *info, u16 b, u16 c, void *data, u16 len) {
    UnkWork_02042e94 *w;
    u16 i;

    data_022c4364 = Task_Create(func_02042684, func_02042670, 0, 1, 0xfffe, sizeof(UnkWork_02042e94));
    w = data_022c4364->work;
    {
        vu16 zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_02042e94));
    }
    w->fn = func_020415a8;
    w->unk4 = 2;
    w->unk8 = 4;
    MI_CpuCopy8(info + 2, w->mac, 6);
    w->unk5e = info[0];
    w->unk192a = 0;
    w->unk16 = 0x19;
    w->unk1af0 = b;
    w->unk1af2 = c;
    for (i = 0; i < info[1]; i++)
        w->mask = (w->mask << 1) | 1;
    func_0203e980(((u16 *)info)[1] - 1);
    WH_Initialize();
    WH_SetGgid(0x51);
    if (data != NULL && len != 0)
        MI_CpuCopy8(data, data_022c4380, len);
    gSystemFlags &= ~0x100;
    gSystemFlags |= 0x200;
    data_0224ff40 = func_02041300;
}
