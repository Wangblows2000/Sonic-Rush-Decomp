/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02049070();
s32 func_0203ac70();
s32 func_02039b48();
s32 func_020395f4();
s32 func_02002ab0();
s32 GXS_SetGraphicsMode();
extern u8 data_0208922c[];
extern u8 data_02087d04[];
extern u8 data_02087d44[];
extern u8 data_0208926c[];
s32 GXx_SetMasterBrightness_();

void func_ov001_022d91c4(void) {
    *(volatile u16 *)0x5000000 = 0xffff;
    *(volatile u16 *)0x5000400 = 0xffff;
    *(volatile u32 *)0x4000000 = (*(volatile u32 *)0x4000000 & -7937);
    *(volatile u32 *)0x4001000 = (*(volatile u32 *)0x4001000 & -7937);
    *(volatile u16 *)0x4000304 = ((*(volatile u16 *)0x4000304 & -527) | 0x20e);
    s32 t0 = (*(volatile u16 *)0x4000304 | 0x8000);
    *(volatile u16 *)0x4000304 = t0;
    func_02049070(0x8000, t0, 0x4000304);
    func_0203ac70(0);
    func_02039b48();
    func_020395f4();
    func_02002ab0(1, 0, 0);
    *(volatile u32 *)0x4000000 = (*(volatile u32 *)0x4000000 & 0xc7ffffff);
    *(volatile u32 *)0x4000000 = (*(volatile u32 *)0x4000000 & 0xf8ffffff);
    *(volatile u16 *)0x4000008 = ((*(volatile u16 *)0x4000008 & 0x43) | 0x4084);
    *(volatile u16 *)0x400000a = ((*(volatile u16 *)0x400000a & 0x43) | 0x4084);
    *(volatile u16 *)0x400000c = ((*(volatile u16 *)0x400000c & 0x43) | 0x4084);
    s32 t1 = ((*(volatile u16 *)0x400000e & 0x43) | 0x288);
    *(volatile u16 *)0x400000e = t1;
    GXS_SetGraphicsMode(0, 0x43, 0x400000e, t1);
    *(volatile u16 *)0x4001008 = ((*(volatile u16 *)0x4001008 & 0x43) | 0x4088);
    *(volatile u16 *)0x400100a = ((*(volatile u16 *)0x400100a & 0x43) | 0x4088);
    *(volatile u16 *)0x400100c = ((*(volatile u16 *)0x400100c & 0x43) | 0x4088);
    *(volatile u16 *)0x400100e = ((*(volatile u16 *)0x400100e & 0x43) | 0x28c);
    *(volatile u16 *)0x4000008 = (*(volatile u16 *)0x4000008 & ~3);
    *(volatile u16 *)0x400000a = ((*(volatile u16 *)0x400000a & ~3) | 1);
    *(volatile u16 *)0x400000c = ((*(volatile u16 *)0x400000c & ~3) | 2);
    *(volatile u16 *)0x400000e = ((*(volatile u16 *)0x400000e & ~3) | 3);
    *(volatile u16 *)0x4001008 = (*(volatile u16 *)0x4001008 & ~3);
    *(volatile u16 *)0x400100a = ((*(volatile u16 *)0x400100a & ~3) | 1);
    *(volatile u16 *)0x400100c = ((*(volatile u16 *)0x400100c & ~3) | 2);
    *(volatile u16 *)0x400100e = ((*(volatile u16 *)0x400100e & ~3) | 3);
    *(u16 *)data_0208922c = 0xff60;
    *(u16 *)((u8 *)data_0208922c + 0x2) = 0;
    *(u16 *)((u8 *)data_0208922c + 0x4) = 0xff40;
    *(u16 *)((u8 *)data_0208922c + 0x6) = 0;
    *(u16 *)((u8 *)data_0208922c + 0x8) = 0xff20;
    *(u16 *)((u8 *)data_0208922c + 0xa) = 0;
    *(u16 *)((u8 *)data_0208922c + 0xc) = 0;
    *(u16 *)((u8 *)data_0208922c + 0xe) = 0;
    *(u16 *)data_02087d04 = 0xff60;
    *(u16 *)((u8 *)data_02087d04 + 0x2) = 0;
    *(u16 *)((u8 *)data_02087d04 + 0x4) = 0xff40;
    *(u16 *)((u8 *)data_02087d04 + 0x6) = 0;
    *(u16 *)((u8 *)data_02087d04 + 0x8) = 0xff20;
    *(u16 *)((u8 *)data_02087d04 + 0xa) = 0;
    *(u16 *)((u8 *)data_02087d04 + 0xc) = 0;
    *(u16 *)((u8 *)data_02087d04 + 0xe) = 0;
    *(volatile u32 *)0x4000010 = ((*(u16 *)data_0208922c & 0x1ff) | ((*(u16 *)((u8 *)data_0208922c + 0x2) << 0x10) & 0x1ff0000));
    *(volatile u32 *)0x4000014 = ((*(u16 *)((u8 *)data_0208922c + 0x4) & 0x1ff) | ((*(u16 *)((u8 *)data_0208922c + 0x6) << 0x10) & 0x1ff0000));
    *(volatile u32 *)0x4000018 = ((*(u16 *)((u8 *)data_0208922c + 0x8) & 0x1ff) | ((*(u16 *)((u8 *)data_0208922c + 0xa) << 0x10) & 0x1ff0000));
    *(volatile u32 *)0x400001c = ((*(u16 *)((u8 *)data_0208922c + 0xc) & 0x1ff) | ((*(u16 *)((u8 *)data_0208922c + 0xe) << 0x10) & 0x1ff0000));
    *(volatile u32 *)0x4001010 = ((*(u16 *)data_02087d04 & 0x1ff) | ((*(u16 *)((u8 *)data_02087d04 + 0x2) << 0x10) & 0x1ff0000));
    *(volatile u32 *)0x4001014 = ((*(u16 *)((u8 *)data_02087d04 + 0x4) & 0x1ff) | ((*(u16 *)((u8 *)data_02087d04 + 0x6) << 0x10) & 0x1ff0000));
    *(volatile u32 *)0x4001018 = ((*(u16 *)((u8 *)data_02087d04 + 0x8) & 0x1ff) | ((*(u16 *)((u8 *)data_02087d04 + 0xa) << 0x10) & 0x1ff0000));
    s32 t2 = ((*(u16 *)((u8 *)data_02087d04 + 0xc) & 0x1ff) | ((*(u16 *)((u8 *)data_02087d04 + 0xe) << 0x10) & 0x1ff0000));
    *(volatile u32 *)0x400101c = t2;
    *(u16 *)((u8 *)data_02087d44 + 0x8) = 0x10;
    s32 t3 = *(s16 *)((u8 *)data_02087d44 + 8);
    *(u16 *)((u8 *)data_0208926c + 0x8) = t3;
    GXx_SetMasterBrightness_(0x400006c, 0x10, t3, t2);
    GXx_SetMasterBrightness_(0x400106c, 0x10);
}

#pragma thumb reset
