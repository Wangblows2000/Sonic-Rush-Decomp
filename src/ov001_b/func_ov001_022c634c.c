#include <nitro/types.h>

typedef struct UnkStruct_022c634c_br {
    u8 pad0[0x48];
    s16 unk48;
} UnkStruct_022c634c_br;

typedef struct UnkStruct_022c634c_f754 {
    u8 pad[0x80];
    vu32 unk80;
    vu32 unk84;
} UnkStruct_022c634c_f754;

extern UnkStruct_022c634c_br data_0208922c;
extern UnkStruct_022c634c_br data_02087d04;
extern UnkStruct_022c634c_f754 data_0208f754;

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
void func_0203a6bc(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_02039d08(s32 i);
void func_0203a9d0(s32 a);
void func_02039ef8(s32 a);
void func_02039d40(s32 a);
void func_02002ab0(s32 a, s32 b, s32 c);
void func_02004040(u32 a, u32 b, u16 c, u32 d, BOOL e);
void func_0203a288(s32 a, s32 b, s32 c, s32 d, s32 e);
void func_0203a43c(s32 a);
void GXS_SetGraphicsMode(u32 mode);
void GXx_SetMasterBrightness_(vu16 *reg, s32 b);

void func_ov001_022c634c(void) {
    *(vu16 *)0x4000304 = (*(vu16 *)0x4000304 & ~0x20e) | 0x20e;
    *(vu16 *)0x4000304 |= 0x8000;
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
    func_0203a6bc(0x40, 0x100010, 0x40, 0, 0x100);
    func_02039d08(0);
    func_0203a9d0(0x10);
    func_02039ef8(0xb);
    func_02039d40(0x20);
    func_02002ab0(1, 0, 1);
    *(vu32 *)0x4000000 &= ~0x38000000;
    *(vu32 *)0x4000000 &= ~0x7000000;
    data_0208f754.unk80 = 0;
    data_0208f754.unk84 = 1;
    *(vu16 *)0x4000060 = *(vu16 *)0x4000060 & ~0x3002;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 0x10;
    *(vu16 *)0x4000060 = *(vu16 *)0x4000060 & 0xcffb;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 8;
    *(vu16 *)0x4000060 = (*(vu16 *)0x4000060 & ~0x3000) | 0x20;
    func_02004040(0, 0, 0x7fff, 0, 0);
    *(vu32 *)0x4000540 = (data_0208f754.unk84 << 1) | data_0208f754.unk80;
    *(vu32 *)0x4000580 = 0xbfff0000;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & 0x43) | 0x600;
    *(vu16 *)0x4000008 = (*(vu16 *)0x4000008 & ~3) | 1;
    *(vu16 *)0x400000a = (*(vu16 *)0x400000a & ~3) | 3;
    *(vu16 *)0x400000c = *(vu16 *)0x400000c & ~3;
    *(vu16 *)0x400000e = (*(vu16 *)0x400000e & ~3) | 2;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1100;
    func_0203a288(0x100, 0x100010, 0x40, 0, 0x100);
    func_02039d08(1);
    func_0203a43c(4);
    GXS_SetGraphicsMode(0);
    *(vu16 *)0x4001008 = (*(vu16 *)0x4001008 & 0x43) | 0x2788;
    *(vu16 *)0x400100a = (*(vu16 *)0x400100a & 0x43) | 0x2698;
    *(vu16 *)0x400100c = (*(vu16 *)0x400100c & 0x43) | 0x700;
    *(vu16 *)0x400100e = (*(vu16 *)0x400100e & 0x43) | 0x900;
    *(vu16 *)0x4001008 = (*(vu16 *)0x4001008 & ~3) | 1;
    *(vu16 *)0x400100a = (*(vu16 *)0x400100a & ~3) | 3;
    *(vu16 *)0x400100c = *(vu16 *)0x400100c & ~3;
    *(vu16 *)0x400100e = (*(vu16 *)0x400100e & ~3) | 2;
    data_0208922c.unk48 = 0x10;
    data_02087d04.unk48 = 0x10;
    GXx_SetMasterBrightness_((vu16 *)0x400006c, data_0208922c.unk48);
    GXx_SetMasterBrightness_((vu16 *)0x400106c, data_02087d04.unk48);
    *(vu32 *)0x4001000 = (*(vu32 *)0x4001000 & ~0x1f00) | 0x1c00;
}
