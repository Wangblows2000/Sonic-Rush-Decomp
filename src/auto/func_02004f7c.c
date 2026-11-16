/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_02077964[];
s32 func_02007b3c();
extern u8 data_0207d010[];
s32 func_020034cc();
extern u8 data_0207d018[];
extern u8 data_0207d014[];
extern u8 data_0207d004[];

void func_02004f7c(void) {
    s32 t0 = *(u32 *)data_02077964;
    if (t0 != -1) {
        func_02007b3c(t0, -1);
    }
    func_020034cc(*(u32 *)data_0207d010);
    *(u32 *)data_0207d018 = 0;
    *(u32 *)data_0207d014 = 0;
    *(u32 *)data_0207d004 = 0;
    *(u32 *)data_0207d010 = 0;
}
