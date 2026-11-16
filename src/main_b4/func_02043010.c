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
    u8 pad24[0x192a - 0x24];
    u16 unk192a;
    u8 pad192c[0x1aee - 0x192c];
    u16 unk1aee;
    u16 unk1af0;
    u16 unk1af2;
    u8 pad1af4[0x1cf4 - 0x1af4];
} UnkWork_02043010;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02043010 *work;
} UnkTask_02043010;

extern UnkTask_02043010 *data_022c4364;
extern u8 data_022c43a0[];
extern u32 data_02087cb8;
extern void (*data_0224ff40)(void);

UnkTask_02043010 *func_020372d0(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void func_02042684(void);
void func_02042670(void);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void func_02041d8c(void);
void func_0203e980(u16 a);
void func_0203e480(void);
void func_0203e9bc(s32 a);
void func_02042608(void);
void func_0203df04(void (*f)(void));
void MI_CpuCopy8(const void *src, void *dst, u32 size);
void func_02041300(void);

void func_02043010(u16 *info, void *a, u16 b, u16 c, void *data, u16 len) {
    UnkWork_02043010 *w;
    u16 i;

    data_022c4364 = func_020372d0(func_02042684, func_02042670, 0, 1, 0xfffe, sizeof(UnkWork_02043010));
    w = data_022c4364->work;
    {
        vu16 zero = 0;
        MIi_CpuClear16(zero, w, sizeof(UnkWork_02043010));
    }
    w->fn = func_02041d8c;
    w->unk4 = 2;
    w->unk8 = 2;
    w->unk10 = info[0];
    w->unk12 = info[2];
    w->unk16 = 0x19;
    w->unk192a = 0xffff;
    w->unk1af0 = b;
    w->unk1af2 = c;
    w->unk1c = a;
    for (i = 0; i < info[1]; i++)
        w->mask = (w->mask << 1) | 1;
    func_0203e980(((u16 *)info)[1] - 1);
    func_0203e480();
    func_0203e9bc(0x51);
    func_0203df04(func_02042608);
    if (data != NULL && len != 0) {
        w->unk20 = len;
        MI_CpuCopy8(data, data_022c43a0, len);
    }
    data_02087cb8 &= ~0x100;
    data_02087cb8 |= 0x200;
    data_0224ff40 = func_02041300;
}
