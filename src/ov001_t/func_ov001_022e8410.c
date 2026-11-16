#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[0x1cc8 - 4];
    s32 unk1cc8;
    s32 unk1ccc;
    s32 unk1cd0;
    u8 pad1cd4[0x1cec - 0x1cd4];
} UnkWork_022e8410;

typedef struct {
    u8 pad[0x10];
    UnkWork_022e8410 *work;
} UnkTask_022e8410;

UnkTask_022e8410 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void func_ov001_022e83bc(void);
void func_ov001_022e8378(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
UnkWork_022e8410 *func_ov001_022e8410(void) {
    UnkWork_022e8410 *w;
    vu32 zero;

    w = Task_Create(func_ov001_022e83bc, func_ov001_022e8378, 0, 0x1000, 0x1000, sizeof(UnkWork_022e8410))->work;
    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkWork_022e8410));
    w->unk0 = 0;
    w->unk1cc8 = 3;
    w->unk1ccc = -1;
    w->unk1cd0 = -1;
    return w;
}
#pragma thumb reset
