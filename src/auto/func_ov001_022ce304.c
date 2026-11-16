/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02049070();
extern u8 data_0208922c[];
extern u8 data_02087d04[];

void func_ov001_022ce304(void) {
    *(volatile u16 *)0x5000000 = 0xffff;
    *(volatile u16 *)0x5000400 = 0xffff;
    *(volatile u32 *)0x4000000 = (*(volatile u32 *)0x4000000 & -7937);
    s32 t0 = (*(volatile u32 *)0x4001000 & -7937);
    *(volatile u32 *)0x4001000 = t0;
    func_02049070(-7937, 0x4001000, t0);
    *(u16 *)data_0208922c = 0;
    *(u16 *)((u8 *)data_0208922c + 0x2) = 0;
    *(u16 *)((u8 *)data_0208922c + 0x4) = 0;
    *(u16 *)((u8 *)data_0208922c + 0x6) = 0;
    *(u16 *)((u8 *)data_0208922c + 0x8) = 0;
    *(u16 *)((u8 *)data_0208922c + 0xa) = 0;
    *(u16 *)((u8 *)data_0208922c + 0xc) = 0;
    *(u16 *)((u8 *)data_0208922c + 0xe) = 0;
    *(u16 *)data_02087d04 = 0;
    *(u16 *)((u8 *)data_02087d04 + 0x2) = 0;
    *(u16 *)((u8 *)data_02087d04 + 0x4) = 0;
    *(u16 *)((u8 *)data_02087d04 + 0x6) = 0;
    *(u16 *)((u8 *)data_02087d04 + 0x8) = 0;
    *(u16 *)((u8 *)data_02087d04 + 0xa) = 0;
    *(u16 *)((u8 *)data_02087d04 + 0xc) = 0;
    *(u16 *)((u8 *)data_02087d04 + 0xe) = 0;
}

#pragma thumb reset
