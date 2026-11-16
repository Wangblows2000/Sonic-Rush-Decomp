#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[0x16c - 4];
} UnkWork_022d4f30;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022d4f30 *work;
} UnkTask_022d4f30;

UnkTask_022d4f30 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov001_022d4c28(void);
void func_ov001_022d4d68(void);

#pragma thumb on
UnkWork_022d4f30 *func_ov001_022d4f30(void) {
    UnkWork_022d4f30 *w;

    w = Task_Create(func_ov001_022d4c28, func_ov001_022d4d68, 0, 0x1000, 0, sizeof(UnkWork_022d4f30))->work;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, w, sizeof(UnkWork_022d4f30));
    }
    w->unk0 = 0;
    return w;
}
#pragma thumb reset
