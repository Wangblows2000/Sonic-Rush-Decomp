#include <nitro/types.h>

#define REG16(a) (*(vu16 *)(a))
#define REG32(a) (*(vu32 *)(a))

typedef struct {
    vs32 unk0;
    vs32 unk4;
} UnkStruct_0208f7d4;

extern UnkStruct_0208f7d4 data_0208f7d4;

void func_02002f60(void);
void func_02002f4c(void);
void func_02002f28(void);
void func_02002f04(void);
void func_02002ef0(void);
void func_02002edc(void);
void func_02002ec8(void);
void func_02002e8c(void);
void func_02002e78(void);
void func_02002e50(void);
void func_02002e28(void);
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
void GXS_SetGraphicsMode(s32 mode);
void func_02039d08(s32 a);
void func_02039b48(void);
void func_020395f4(void);
void func_02004040(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_ov000_02355fc0(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on

void func_ov000_02356054(BOOL arg) {
    vu32 z0;
    vu32 z1;
    vu32 z2;
    vu32 z3;
    vu32 z4;
    vu32 z5;

    REG32(0x4000000) = REG32(0x4000000) & ~0x1f00;
    REG32(0x4001000) = REG32(0x4001000) & ~0x1f00;
    func_02002f60();
    func_02002f4c();
    func_02002f28();
    func_02002f04();
    func_02002ef0();
    func_02002edc();
    func_02002ec8();
    func_02002e8c();
    func_02002e78();
    func_02002e50();
    func_02002e28();
    func_02002eb4();
    func_02002ea0();
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
    REG16(0x4000304) = (REG16(0x4000304) & ~0x20e) | 0x20e;
    REG16(0x4000304) &= ~0x8000;
    func_02002ab0(1, 3, 1);
    REG32(0x4000000) &= ~0x38000000;
    REG32(0x4000000) &= ~0x07000000;
    GXS_SetGraphicsMode(0);
    func_0203a6bc(0x10, 0x100010, 0x40, 0, 0x400);
    func_0203a504(0);
    func_02039d08(0);
    func_0203a9d0(2);
    func_0203a590(0x40);
    func_0203a288(8, 0x200010, 0x40, 0, 0x400);
    func_0203a18c(0);
    func_02039d08(1);
    func_0203a43c(4);
    func_0203a1f8(0x80);
    func_02039ef8(1);
    func_02039d40(0x20);
    if (arg) {
        func_02039b48();
        func_020395f4();
    }
    data_0208f7d4.unk0 = 0;
    data_0208f7d4.unk4 = 0;
    REG16(0x4000060) &= ~0x3002;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x10;
    REG16(0x4000060) &= (u16)~0x3004;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x8;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x20;
    func_02004040(0, 0, 0x7fff, 0, 0);
    {
        s32 zw = data_0208f7d4.unk4;
        s32 am = data_0208f7d4.unk0;
        REG32(0x4000540) = am | (zw << 1);
    }
    REG32(0x4000580) = 0xbfff0000;
    REG16(0x400000a) = (REG16(0x400000a) & 0x43) | 0x2088;
    REG16(0x400000c) = (REG16(0x400000c) & 0x43) | 0x290;
    REG16(0x400000e) = (REG16(0x400000e) & 0x43) | 0x4418;
    REG16(0x4001008) = (REG16(0x4001008) & 0x43) | 0x4088;
    REG16(0x400100c) = (REG16(0x400100c) & 0x43) | 0x498;
    REG16(0x400100e) = (REG16(0x400100e) & 0x43) | 0x69c;
    REG16(0x400000a) = (REG16(0x400000a) & ~3);
    REG16(0x4000008) = (REG16(0x4000008) & ~3) | 1;
    REG16(0x400000e) = (REG16(0x400000e) & ~3) | 2;
    REG16(0x400000c) = (REG16(0x400000c) & ~3) | 3;
    REG16(0x4001008) = (REG16(0x4001008) & ~3);
    REG16(0x400100a) = (REG16(0x400100a) & ~3) | 1;
    REG16(0x400100c) = (REG16(0x400100c) & ~3) | 2;
    REG16(0x400100e) = (REG16(0x400100e) & ~3) | 3;
    func_ov000_02355fc0();
    z0 = 0;
    MIi_CpuClear32(z0, (void *)0x6000000, 0x80000);
    z1 = 0;
    MIi_CpuClear32(z1, (void *)0x6200000, 0x20000);
    z2 = 0;
    MIi_CpuClear32(z2, (void *)0x5000000, 0x200);
    z3 = 0;
    MIi_CpuClear32(z3, (void *)0x5000400, 0x200);
    z4 = 0;
    MIi_CpuClear32(z4, (void *)0x5000200, 0x200);
    z5 = 0;
    MIi_CpuClear32(z5, (void *)0x5000600, 0x200);
    REG32(0x4000000) = (REG32(0x4000000) & ~0x1f00) | 0x1d00;
    REG32(0x4001000) = (REG32(0x4001000) & ~0x1f00) | 0x1d00;
}

#pragma thumb reset
