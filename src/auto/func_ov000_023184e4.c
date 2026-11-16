/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236a26c[];
s32 func_020686dc();
extern u8 data_ov000_0236a274[];
s32 func_020683e4();
extern u8 data_ov000_0236a27c[];
extern u8 data_ov000_0236a284[];
extern u8 data_ov000_0236a28c[];
extern u8 data_ov000_0236a294[];
extern u8 data_ov000_0236a29c[];
s32 func_02039bb0();
s32 func_ov000_022fb9c8();

void func_ov000_023184e4(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    func_020686dc(data_ov000_0236a26c);
    func_020686dc(data_ov000_0236a26c);
    func_020683e4(data_ov000_0236a274, 0);
    func_020683e4(data_ov000_0236a27c, 1);
    func_020683e4(data_ov000_0236a284, 0);
    func_020683e4(data_ov000_0236a28c, 1);
    func_020686dc(data_ov000_0236a26c);
    func_020683e4(data_ov000_0236a294, 0);
    func_020683e4(data_ov000_0236a29c, 1);
    func_020686dc(data_ov000_0236a26c);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x3f0);
    if (t1 != 0) {
        func_02039bb0(0, t1);
    }
    s32 t2 = *(u32 *)((u8 *)t0 + 0x438);
    if (t2 != 0) {
        func_02039bb0(1, t2);
    }
    func_ov000_022fb9c8(p0);
}
