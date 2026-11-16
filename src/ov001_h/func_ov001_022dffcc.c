#include <nitro/types.h>

#define REG16(a) (*(vu16 *)(a))
#define REG32(a) (*(vu32 *)(a))

typedef struct UnkStruct_0208f754 {
    u8 pad0[0x80];
    s32 unk80;
    vs32 unk84;
} UnkStruct_0208f754;

typedef struct UnkStruct_022dffcc_sub {
    u8 pad0[0x68];
    u32 unk68;
    u8 pad6C[0x92 - 0x6c];
    u16 unk92;
    u16 unk94;
} UnkStruct_022dffcc_sub;

typedef struct UnkMtx22_022dffcc {
    s32 m[4];
} UnkMtx22_022dffcc;

extern UnkStruct_0208f754 data_0208f754;
extern UnkStruct_022dffcc_sub data_02087d04;
extern u8 data_02087d2c[];

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
void func_02039ef8(s32 a);
void func_02039b48(void);
void func_02039d40(s32 a);
void func_020395f4(void);
void func_02002ab0(s32 a, s32 b, s32 c);
void func_02004040(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_02039d08(s32 a);
void func_02003290(s32 a);
void func_0203a43c(s32 a);
void GXS_SetGraphicsMode(s32 mode);
void func_02000cb4(UnkMtx22_022dffcc *m);
void func_02003d94(void *p, UnkMtx22_022dffcc *m, s32 x, s32 y, s32 a, s32 b);

void func_ov001_022dffcc(void) {
    UnkMtx22_022dffcc mtx;

    REG16(0x4000304) = (REG16(0x4000304) & ~0x20e) | 0x20e;
    REG16(0x4000304) |= 0x8000;
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
    func_02039ef8(3);
    func_02039b48();
    func_02039d40(0x10);
    func_020395f4();
    func_02002ab0(1, 5, 1);
    REG32(0x4000000) &= ~0x38000000;
    REG32(0x4000000) &= ~0x07000000;
    data_0208f754.unk80 = 0;
    data_0208f754.unk84 = 1;
    REG16(0x4000060) &= ~0x3002;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x10;
    REG16(0x4000060) &= (u16)~0x3004;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x8;
    REG16(0x4000060) = (REG16(0x4000060) & ~0x3000) | 0x20;
    func_02004040(0, 0, 0x7fff, 0, 0);
    {
        s32 zw = data_0208f754.unk84;
        s32 am = data_0208f754.unk80;
        REG32(0x4000540) = am | (zw << 1);
    }
    REG32(0x4000580) = 0xbfff0000;
    REG16(0x4000008) &= ~3;
    REG32(0x4000000) = (REG32(0x4000000) & ~0x1f00) | 0x100;
    func_02039d08(1);
    data_02087d04.unk94 = 2;
    data_02087d04.unk92 = 0x80;
    data_02087d04.unk68 = 0x6860000;
    func_02003290(8);
    REG32(0x4001000) = (REG32(0x4001000) & ~0x60) | 0x20;
    func_0203a43c(4);
    GXS_SetGraphicsMode(5);
    REG16(0x400100c) = (REG16(0x400100c) & 0x43) | 0x4084;
    func_02000cb4(&mtx);
    func_02003d94(data_02087d2c, &mtx, 0, 0, 0, 0);
    REG16(0x400100c) &= ~3;
    REG32(0x4001000) = (REG32(0x4001000) & ~0x1f00) | 0x1400;
}
