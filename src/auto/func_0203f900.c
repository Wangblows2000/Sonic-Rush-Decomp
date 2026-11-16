/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022c1980[];
s32 OS_Terminate();
s32 func_020405e0();
extern u8 data_022c1968[];
extern u8 data_022c194c[];
extern u8 data_022c19e0[];
extern u8 data_022c1940[];
s32 func_0203f7f8();

s32 func_0203f900(s32 p0, u8 *p1, s32 p2) {
    s32 t0 = *(u32 *)data_022c1980;
    if (t0 == 4) {
        OS_Terminate(t0, p1, p2, data_022c1980);
    }
    func_020405e0(2);
    *(u32 *)data_022c1968 = p0;
    *(u16 *)data_022c194c = p2;
    *(u16 *)((u8 *)data_022c19e0 + 0x4) = 0;
    *(u16 *)data_022c1940 = 0;
    *(u16 *)((u8 *)data_022c19e0 + 0xc) = *(u16 *)((u8 *)p1 + 0x4);
    *(u16 *)((u8 *)data_022c19e0 + 0xa) = *(u16 *)((u8 *)p1 + 0x2);
    s32 t1 = *(u16 *)p1;
    *(u16 *)((u8 *)data_022c19e0 + 0x8) = t1;
    s32 t2 = func_0203f7f8(t1, data_022c19e0, data_022c194c, data_022c1968);
    if (t2 != 0) {
        return 1;
    }
    func_020405e0(9);
    return 0;
}
