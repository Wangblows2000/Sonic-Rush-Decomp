/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_ov000_0236076c();
s32 func_ov000_02360974();
s32 func_ov000_02360950();
s32 func_ov000_02360870();
s32 func_ov000_02360790();
s32 func_ov000_0235fc48();
s32 func_ov000_02360748();
s32 func_ov000_02360724();
s32 func_ov000_023606c4();
s32 func_ov000_02360268();
s32 func_ov000_0236011c();
s32 func_ov000_0235ffd0();
s32 func_ov000_02360264();

void func_ov000_023611f8(u8 *p0) {
    func_ov000_0236076c(p0, 8, 0x88);
    func_ov000_0236076c(p0, 0x98, 0x88);
    if (*(u32 *)((u8 *)p0 + 0x28) == 0) {
        func_ov000_02360974(p0, 8, 0x98);
        func_ov000_02360950(p0, 0x98, 0x98);
        func_ov000_02360870(p0, 0x38, 0x58, 0x1000);
        func_ov000_02360790(p0, 0xc8, 0x58, 0x1000);
    } else {
        func_ov000_02360950(p0, 8, 0x98);
        func_ov000_02360974(p0, 0x98, 0x98);
        func_ov000_02360790(p0, 0x38, 0x58, 0x1000);
        func_ov000_02360870(p0, 0xc8, 0x58, 0x1000);
    }
    func_ov000_0235fc48(p0, 8, 0, 0);
    func_ov000_02360748(p0, 8, 0);
    func_ov000_0235fc48(p0, 0x98, 0, 1);
    func_ov000_02360748(p0, 0x98, 0);
    func_ov000_02360724(p0, 0x80, 0x58);
    func_ov000_023606c4(p0, 0x80, 0x58, 0x1000);
    func_ov000_02360268(p0);
    if (*(u32 *)((u8 *)p0 + 0x28) == 0) {
        func_ov000_0236011c(p0, 0x38, 0x90, 1);
        func_ov000_0235ffd0(p0, 0xc8, 0x90, 0);
    } else {
        func_ov000_0235ffd0(p0, 0x38, 0x90, 1);
        func_ov000_0236011c(p0, 0xc8, 0x90, 0);
    }
    func_ov000_02360264(p0);
}

#pragma thumb reset
