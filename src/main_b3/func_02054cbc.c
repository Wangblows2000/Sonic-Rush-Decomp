#include <nitro/types.h>

typedef struct { u32 timer; } UnkWork_02054cbc;
typedef struct { u8 pad0[0x10]; UnkWork_02054cbc *work; } UnkTask_02054cbc;
typedef struct { u8 pad0[8]; s16 unk8; } UnkBlend_02054cbc;
extern UnkTask_02054cbc *data_022b4574;
extern UnkBlend_02054cbc data_0208926c;
extern UnkBlend_02054cbc data_02087d44;
extern s32 data_02087ccc;
extern u32 data_02087cb8;

void GXx_SetMasterBrightness_(vu16 *reg, s32 v);
void func_02037134(void);
void func_020521b4(void);
void func_020550ec(void);

#pragma thumb on
void func_02054cbc(void) {
    UnkWork_02054cbc *w = data_022b4574->work;

    data_0208926c.unk8 = w->timer << 1;
    data_02087d44.unk8 = w->timer << 1;
    w->timer++;
    if (w->timer >= 8) {
        data_0208926c.unk8 = 0x10;
        data_02087d44.unk8 = 0x10;
        GXx_SetMasterBrightness_((vu16 *)0x0400006c, data_0208926c.unk8);
        GXx_SetMasterBrightness_((vu16 *)0x0400106c, data_02087d44.unk8);
        func_02037134();
        func_020521b4();
        data_02087ccc = 0;
        data_02087cb8 &= ~0x10;
        func_020550ec();
    }
}
#pragma thumb reset
