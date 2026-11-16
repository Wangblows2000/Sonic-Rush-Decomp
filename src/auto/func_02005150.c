/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_020030e4();
extern u8 data_02072580[];
extern u8 data_02072582[];
extern u8 data_02072584[];
extern u8 data_0207d010[];
extern u8 data_0207d004[];
extern u8 data_0207d014[];
extern u8 data_0207d018[];

void func_02005150(void) {
    s32 t0 = func_020030e4();
    s32 t1 = (*(u16 *)((u8 *)data_02072580 + (t0 * 6)) << 0xc);
    s32 t2 = (*(u16 *)((u8 *)data_02072582 + (t0 * 6)) << 0xc);
    *(u32 *)data_0207d010 = t0;
    *(u32 *)data_0207d004 = t1;
    *(u32 *)data_0207d014 = t2;
    *(u32 *)data_0207d018 = (*(u16 *)((u8 *)data_02072584 + (t0 * 6)) << 0xc);
}
