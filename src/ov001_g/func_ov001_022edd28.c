#include <nitro/types.h>

typedef struct UnkWork_022edd28 {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    u8 pad14[0x10];
    s32 unk24;
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    u8 pad38[0x12d4 - 0x38];
} UnkWork_022edd28;

typedef struct UnkTask_022edd28 {
    u8 pad0[0x10];
    UnkWork_022edd28 *work;
} UnkTask_022edd28;

UnkTask_022edd28 *func_020372d0(void (*func)(void), void (*dtor)(void), u16 prio, u16 id, u16 unk1a, u32 workSize);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov001_022ed818(void);
void func_ov001_022ed964(void);
void func_ov001_022ec38c(UnkWork_022edd28 *w);

#pragma thumb on
UnkWork_022edd28 *func_ov001_022edd28(void) {
    UnkWork_022edd28 *w;
    vu16 zero;

    w = func_020372d0(func_ov001_022ed818, func_ov001_022ed964, 0, 0x1000, 0x1000, sizeof(UnkWork_022edd28))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_022edd28));
    w->unk0 = 0;
    w->unk2C = 0;
    w->unk28 = 0;
    w->unk30 = 0;
    w->unk34 = 0;
    w->unk4 = 6;
    w->unk8 = 6;
    w->unkC = 0;
    w->unk10 = 0;
    w->unk24 = 0;
    func_ov001_022ec38c(w);
    if (w->unk28 > 4) {
        w->unk2C = 1;
        w->unk34 = (w->unk28 - 4) * 16;
    }
    return w;
}
#pragma thumb reset
