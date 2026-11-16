/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_022b4574[];
s32 func_0203344c();
s32 func_02032d2c();
s32 func_02063890();
void func_02063c98();
extern u8 data_022c4dbc[];

void func_02063950(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = (*(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x688);
    func_0203344c((*(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x688), 0, 0, *(u32 *)*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10));
    func_02032d2c(t1);
    func_02063890(t0);
    if ((*(u32 *)((u8 *)t1 + 0x2c) & 2) == 0) {
        return;
    }
    *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_02063c98;
    *(u16 *)data_022c4dbc = 0;
    *(u32 *)((u8 *)t0 + 0xc) = -1;
}
