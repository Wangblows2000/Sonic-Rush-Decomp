/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236a988[];
extern u8 data_ov000_0236a990[];
s32 func_020686dc();
s32 func_0206bef4();

void func_ov000_0232d5e4(s32 p0) {
    *(u32 *)data_ov000_0236a988 = 0;
    func_020686dc(data_ov000_0236a990, data_ov000_0236a988, 0);
    func_020686dc(data_ov000_0236a990);
    s32 t0 = func_0206bef4(p0);
}
