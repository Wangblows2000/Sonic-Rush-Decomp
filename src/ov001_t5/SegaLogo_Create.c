#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 bright;
    u8 pad8[0x90 - 8];
} UnkWork_022cfac8;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022cfac8 *work;
} UnkTask_022cfac8;

typedef struct {
    u8 pad0[8];
    s16 bright;
} UnkBright_022cfac8;

extern UnkBright_022cfac8 data_0208926c;

UnkTask_022cfac8 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void SegaLogo_State_Init(void);
void SegaLogo_Destructor(void);

#pragma thumb on
UnkWork_022cfac8 *SegaLogo_Create(void) {
    UnkWork_022cfac8 *w;

    w = Task_Create(SegaLogo_State_Init, SegaLogo_Destructor, 0, 0x1000, 0, sizeof(UnkWork_022cfac8))->work;
    {
        vu16 z = 0;
        MIi_CpuClear16(z, w, sizeof(UnkWork_022cfac8));
    }
    w->unk0 = 0;
    w->bright = data_0208926c.bright;
    return w;
}
#pragma thumb reset
