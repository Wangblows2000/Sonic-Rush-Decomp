#include <nitro/types.h>

#define REG16(a) (*(vu16 *)(a))
#define REG32(a) (*(vu32 *)(a))

typedef struct UnkBright_022c8140 {
    u8 pad0[0x48];
    s16 brightness;
} UnkBright_022c8140;

extern UnkBright_022c8140 data_0208922c;
extern UnkBright_022c8140 data_02087d04;

void func_02002ec8(void);
void func_02002eb4(void);
void func_02002ea0(void);
void func_0203a9d0(s32 a);
void func_0203a6bc(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203a590(s32 a);
void func_0203a504(s32 a);
void func_02039ef8(s32 a);
void func_02039d40(s32 a);
void func_0203a43c(s32 a);
void func_0203a288(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203a1f8(s32 a);
void func_0203a18c(s32 a);
void func_02002ab0(s32 a, s32 b, s32 c);
void func_02039d08(s32 a);
void GXS_SetGraphicsMode(s32 mode);
void GX_DispOn(void);
void GXx_SetMasterBrightness_(vu16 *reg, s32 brightness);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

void func_ov001_022c8140(void) {
    func_0203a9d0(0);
    func_0203a6bc(0, 0x10, 0x40, 0, 0);
    func_0203a590(0);
    func_0203a504(0);
    func_02039ef8(0);
    func_02039d40(0);
    func_02002ec8();
    func_0203a43c(0);
    func_0203a288(0, 0x10, 0x40, 0, 0);
    func_0203a1f8(0);
    func_0203a18c(0);
    func_02002eb4();
    func_02002ea0();
    func_0203a9d0(3);
    func_0203a590(0x60);
    func_02002ab0(1, 3, 0);
    REG16(0x4000008) = (REG16(0x4000008) & 0x43) | 0x9c;
    REG16(0x400000a) = (REG16(0x400000a) & 0x43) | 0x184;
    REG16(0x400000c) = (REG16(0x400000c) & 0x43) | 0x428c;
    REG16(0x400000e) = (REG16(0x400000e) & 0x43) | 0x4414;
    func_0203a288(0x100, 0x10, 0x40, 0, 0x200);
    func_02039d08(1);
    func_0203a43c(4);
    GXS_SetGraphicsMode(0);
    REG16(0x400100a) = (REG16(0x400100a) & 0x43) | 4;
    REG32(0x4000000) &= ~0x38000000;
    REG32(0x4000000) &= ~0x07000000;
    REG16(0x4000008) = (REG16(0x4000008) & ~3);
    REG16(0x400000e) = (REG16(0x400000e) & ~3) | 1;
    REG16(0x400000c) = (REG16(0x400000c) & ~3) | 2;
    REG16(0x400000a) = (REG16(0x400000a) & ~3) | 3;
    REG16(0x400100a) = (REG16(0x400100a) & ~3);
    REG16(0x4001008) = (REG16(0x4001008) & ~3) | 1;
    REG16(0x400100c) = (REG16(0x400100c) & ~3) | 2;
    REG16(0x400100e) = (REG16(0x400100e) & ~3) | 3;
    REG16(0x4000304) |= 0x8000;
    REG16(0x4000304) = (REG16(0x4000304) & ~0x20e) | 0x20e;
    GX_DispOn();
    REG32(0x4001000) |= 0x10000;
    data_0208922c.brightness = 16;
    data_02087d04.brightness = 16;
    GXx_SetMasterBrightness_((vu16 *)0x400006c, 16);
    GXx_SetMasterBrightness_((vu16 *)0x400106c, 16);
    {
        vu32 z = 0;
        MIi_CpuClear32(z, (void *)0x6000000, 0x4000);
    }
}
