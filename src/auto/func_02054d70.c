/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

s32 func_02054b50();
s32 func_02037d60();
s32 func_020522f4();
s32 func_02052104();
s32 func_02050078();
void func_02054d3c();
s32 func_020372d0();

void func_02054d70(void) {
    func_02054b50();
    func_02037d60();
    func_020522f4();
    func_02052104(6);
    func_02050078(0xb, 1, 2, 0, 0, 0, 0, 0x200010);
    func_020372d0((u32)func_02054d3c, 0, 0, 0, 0, 4);
}

#pragma thumb reset
