#include <nitro/types.h>

typedef struct {
    s32 state;
    s32 unk4;
    s32 unk8;
    u8 padc[0x82c - 0xc];
} UnkWork_022d94b4;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d94b4 *work;
} UnkTask_022d94b4;

extern u8 data_022503e0[];

UnkTask_022d94b4 *func_020372d0(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 e);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov001_022d8c44(void);
void func_ov001_022d8c30(void);
s32 func_ov001_022d8e58(void);
void func_ov001_022d9458(UnkWork_022d94b4 *w);
void func_ov001_022d91c4(void);
void func_ov001_022d8e74(UnkWork_022d94b4 *w);
void func_02037d60(void);
void func_020351d8(void *p, s32 a, s32 b);
void func_020522f4(void);
void func_02052168(s32 a);

#pragma thumb on
UnkWork_022d94b4 *func_ov001_022d94b4(void) {
    UnkWork_022d94b4 *w = func_020372d0(func_ov001_022d8c44, func_ov001_022d8c30, 0, 0x1000, 0x1000, sizeof(UnkWork_022d94b4))->work;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, w, sizeof(UnkWork_022d94b4));
    }
    w->state = 9;
    w->unk4 = 0;
    w->unk8 = func_ov001_022d8e58();
    func_ov001_022d9458(w);
    func_ov001_022d91c4();
    func_ov001_022d8e74(w);
    func_02037d60();
    func_020351d8(data_022503e0, 0, 0);
    func_020522f4();
    func_02052168(0);
    return w;
}
#pragma thumb reset
