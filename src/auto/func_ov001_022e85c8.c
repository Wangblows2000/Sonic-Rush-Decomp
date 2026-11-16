/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02002f60();
s32 func_02002f4c();
s32 func_02002f28();
s32 func_02002f04();
s32 func_02002ef0();
s32 func_02002edc();
s32 func_02002ec8();
s32 func_02002e8c();
s32 func_02002e78();
s32 func_02002e50();
s32 func_02002e28();
s32 func_02002eb4();
s32 func_02002ea0();
s32 func_0203a9d0();
s32 func_0203a6bc();
s32 func_0203a590();
s32 func_0203a504();
s32 func_02039ef8();
s32 func_02039d40();
s32 func_0203a43c();
s32 func_0203a288();
s32 func_0203a1f8();
s32 func_0203a18c();
s32 func_02039b48();
s32 func_020395f4();
extern u8 data_02087d04[];
extern u8 data_0208922c[];

void func_ov001_022e85c8(void) {
    *(volatile u16 *)0x5000000 = 0xffff;
    *(volatile u16 *)0x5000400 = 0xffff;
    *(volatile u32 *)0x4000000 = (*(volatile u32 *)0x4000000 & -7937);
    *(volatile u32 *)0x4001000 = (*(volatile u32 *)0x4001000 & -7937);
    *(volatile u16 *)0x4000304 = ((*(volatile u16 *)0x4000304 & -527) | 0x20e);
    s32 t0 = (*(volatile u16 *)0x4000304 | 0x8000);
    *(volatile u16 *)0x4000304 = t0;
    func_02002f60(0x8000, t0, 0x4000304);
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
    func_02039b48();
    func_020395f4();
    *(u16 *)((u8 *)data_02087d04 + 0xe) = 0;
    *(u16 *)((u8 *)data_02087d04 + 0xc) = *(u16 *)((u8 *)data_02087d04 + 0xe);
    *(u16 *)((u8 *)data_02087d04 + 0xa) = *(u16 *)((u8 *)data_02087d04 + 0xc);
    *(u16 *)((u8 *)data_02087d04 + 0x8) = *(u16 *)((u8 *)data_02087d04 + 0xa);
    *(u16 *)((u8 *)data_02087d04 + 0x6) = *(u16 *)((u8 *)data_02087d04 + 0x8);
    *(u16 *)((u8 *)data_02087d04 + 0x4) = *(u16 *)((u8 *)data_02087d04 + 0x6);
    *(u16 *)((u8 *)data_02087d04 + 0x2) = *(u16 *)((u8 *)data_02087d04 + 0x4);
    *(u16 *)data_02087d04 = *(u16 *)((u8 *)data_02087d04 + 0x2);
    *(u16 *)((u8 *)data_0208922c + 0xe) = *(u16 *)data_02087d04;
    *(u16 *)((u8 *)data_0208922c + 0xc) = *(u16 *)((u8 *)data_0208922c + 0xe);
    *(u16 *)((u8 *)data_0208922c + 0xa) = *(u16 *)((u8 *)data_0208922c + 0xc);
    *(u16 *)((u8 *)data_0208922c + 0x8) = *(u16 *)((u8 *)data_0208922c + 0xa);
    *(u16 *)((u8 *)data_0208922c + 0x6) = *(u16 *)((u8 *)data_0208922c + 0x8);
    *(u16 *)((u8 *)data_0208922c + 0x4) = *(u16 *)((u8 *)data_0208922c + 0x6);
    *(u16 *)((u8 *)data_0208922c + 0x2) = *(u16 *)((u8 *)data_0208922c + 0x4);
    *(u16 *)data_0208922c = *(u16 *)((u8 *)data_0208922c + 0x2);
}

#pragma thumb reset
