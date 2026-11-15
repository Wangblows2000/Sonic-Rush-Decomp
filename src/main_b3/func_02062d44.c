#include <nitro/types.h>

typedef struct { u8 pad0[8]; s16 unk8; } UnkBlend_02062d44;
typedef struct { u16 v[8]; } UnkMosaic_02062d44;
extern UnkBlend_02062d44 data_0208926c;
extern UnkBlend_02062d44 data_02087d44;
extern UnkMosaic_02062d44 data_0208922c;
extern UnkMosaic_02062d44 data_02087d04;

void func_0203a9d0(s32 a);
void func_0203a6bc(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203a590(s32 a);
void func_0203a504(s32 a);
void func_02039ef8(s32 a);
void func_02039d40(s32 a);
void func_02002ec8(void);
void func_0203a43c(s32 a);
void func_0203a288(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203a1f8(s32 a);
void func_0203a18c(s32 a);
void func_02002eb4(void);
void func_02002ea0(void);
void func_02002ab0(s32 a, s32 b, s32 c);
void GXS_SetGraphicsMode(s32 mode);
void func_02039d08(s32 a);
void func_02039b48(void);
void func_020395f4(void);
void MIi_CpuClear32(u32 data, void *dest, u32 size);

#pragma thumb on
static inline void Clear32_02062d44(void *dst, u32 size) {
    vu32 z = 0;
    MIi_CpuClear32(z, dst, size);
}

void func_02062d44(void) {
    s16 b;

    *(vu32 *)0x04000000 = *(vu32 *)0x04000000 & ~0x1f00;
    *(vu32 *)0x04001000 = *(vu32 *)0x04001000 & ~0x1f00;
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
    *(vu16 *)0x04000304 = (*(vu16 *)0x04000304 & ~0x20e) | 0x20e;
    *(vu16 *)0x04000304 |= 0x8000;
    func_02002ab0(1, 0, 0);
    *(vu32 *)0x04000000 &= ~0x38000000;
    *(vu32 *)0x04000000 &= ~0x07000000;
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
    func_02039b48();
    func_020395f4();
    *(vu16 *)0x04000008 = (*(vu16 *)0x04000008 & 0x43) | 0x2084;
    *(vu16 *)0x0400000a = (*(vu16 *)0x0400000a & 0x43) | 0x2188;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & 0x43) | 0x28c;
    *(vu16 *)0x0400000e = (*(vu16 *)0x0400000e & 0x43) | 0x390;
    *(vu16 *)0x04001008 = (*(vu16 *)0x04001008 & 0x43) | 0x4084;
    *(vu16 *)0x0400100a = (*(vu16 *)0x0400100a & 0x43) | 0x4188;
    *(vu16 *)0x0400100c = (*(vu16 *)0x0400100c & 0x43) | 0x28c;
    *(vu16 *)0x0400100e = (*(vu16 *)0x0400100e & 0x43) | 0x390;
    *(vu16 *)0x04000008 = (*(vu16 *)0x04000008 & ~3) | 0;
    *(vu16 *)0x0400000a = (*(vu16 *)0x0400000a & ~3) | 1;
    *(vu16 *)0x0400000c = (*(vu16 *)0x0400000c & ~3) | 2;
    *(vu16 *)0x0400000e = (*(vu16 *)0x0400000e & ~3) | 3;
    *(vu16 *)0x04001008 = (*(vu16 *)0x04001008 & ~3) | 0;
    *(vu16 *)0x0400100a = (*(vu16 *)0x0400100a & ~3) | 1;
    *(vu16 *)0x0400100c = (*(vu16 *)0x0400100c & ~3) | 2;
    *(vu16 *)0x0400100e = (*(vu16 *)0x0400100e & ~3) | 3;
    data_0208922c.v[0] = 0;
    data_0208922c.v[1] = 0;
    data_0208922c.v[2] = 0;
    data_0208922c.v[3] = 0;
    data_0208922c.v[4] = 0;
    data_0208922c.v[5] = 0;
    data_0208922c.v[6] = 0;
    data_0208922c.v[7] = 0;
    data_02087d04.v[0] = 0;
    data_02087d04.v[1] = 0;
    data_02087d04.v[2] = 0;
    data_02087d04.v[3] = 0;
    data_02087d04.v[4] = 0;
    data_02087d04.v[5] = 0;
    data_02087d04.v[6] = 0;
    data_02087d04.v[7] = 0;
    b = data_0208926c.unk8;
    if (b != 0x10 && b != -0x10) {
        data_0208926c.unk8 = -0x10;
        data_02087d44.unk8 = -0x10;
    }
    Clear32_02062d44((void *)0x06000000, 0x80000);
    Clear32_02062d44((void *)0x06200000, 0x20000);
    Clear32_02062d44((void *)0x05000000, 0x200);
    Clear32_02062d44((void *)0x05000400, 0x200);
    Clear32_02062d44((void *)0x05000200, 0x200);
    Clear32_02062d44((void *)0x05000600, 0x200);
}
#pragma thumb reset
