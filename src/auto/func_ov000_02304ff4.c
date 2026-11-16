/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369e64[];
s32 func_020686dc();
extern u8 data_ov000_02369e74[];
s32 func_020683e4();
extern u8 data_ov000_02369e7c[];
extern u8 data_ov000_02369e84[];
extern u8 data_ov000_02369e8c[];
extern u8 data_ov000_02369e6c[];
s32 func_ov000_022fb9c8();

void func_ov000_02304ff4(s32 p0) {
    func_020686dc(data_ov000_02369e64);
    func_020683e4(data_ov000_02369e74, 0);
    func_020683e4(data_ov000_02369e7c, 1);
    func_020686dc(data_ov000_02369e64);
    func_020683e4(data_ov000_02369e84, 0);
    func_020683e4(data_ov000_02369e8c, 1);
    func_020686dc(data_ov000_02369e6c);
    func_ov000_022fb9c8(p0);
}
