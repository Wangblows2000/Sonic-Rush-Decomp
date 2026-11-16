#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u32 pad14;
    u32 flags;
} UnkState_020636ec;

typedef struct {
    u32 pad0;
    s32 unk4;
} UnkWork_020636ec;

typedef struct {
    u8 pad0[0x10];
    UnkWork_020636ec *work;
} UnkTask_020636ec;

extern UnkState_020636ec data_022c4560;

UnkTask_020636ec *func_020372d0(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_020634e0(void);
void func_02063400(void);
void func_02062d44(void);
void func_02051598(s32 a);

#pragma thumb on
void func_020636ec(void) {
    UnkWork_020636ec *w;
    vu16 z;

    data_022c4560.unk10 = 1;
    data_022c4560.flags |= 0x20;
    data_022c4560.flags &= ~0x10;
    w = func_020372d0(func_020634e0, func_02063400, 0, 0, 0, 0x1c0)->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x1c0);
    w->unk4 = 1;
    func_02062d44();
    func_02051598(0x40);
}
#pragma thumb reset
