#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    u8 *work;
} UnkTask_02042bbc;

extern UnkTask_02042bbc *data_022c4364;

UnkTask_02042bbc *func_020372d0(void (*fn)(void), void (*fn2)(void), s32 a, s32 b, s32 c, s32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void func_0203e480(void);
void func_0203e9bc(s32 a);
void func_02041298(void);
void func_02041284(void);
void func_02042304(void);

void func_02042bbc(const void *src, u32 v) {
    u8 *w;
    vu16 z;

    data_022c4364 = func_020372d0(func_02041298, func_02041284, 0, 1, 0xfffe, 0x1cf4);
    w = data_022c4364->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x1cf4);
    *(void (**)(void))w = func_02042304;
    *(s32 *)(w + 4) = 3;
    *(s32 *)(w + 8) = 1;
    MIi_CpuCopy16(src, w + 0x1af4, 0x1c);
    *(u32 *)(w + 0x1b10) = v;
    *(u32 *)(w + 0x1b08) = 0x51;
    func_0203e480();
    func_0203e9bc(0x51);
}
