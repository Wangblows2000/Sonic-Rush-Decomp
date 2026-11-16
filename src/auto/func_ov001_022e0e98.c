/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 gZoneActSelectWork[];
s32 func_020533e0();
s32 func_020537b4();

void func_ov001_022e0e98(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gZoneActSelectWork + 0x10);
    if ((*(u32 *)((u8 *)*(u32 *)gZoneActSelectWork + 0x10) & 2) == 0) {
        if ((t0 & 4) == 0) {
            func_020533e0(0, 0xb4, (t0 & 4));
            func_020537b4(3);
            return;
        }
        func_020533e0(3, 0xb4);
        func_020537b4(2);
    }
}

#pragma thumb reset
