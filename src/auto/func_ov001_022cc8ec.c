/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

#pragma thumb on

extern u8 data_022b4574[];
s32 func_ov001_022cc17c();
s32 func_ov001_022cbf30();
s32 func_02050018();
s32 func_0205000c();
void func_ov001_022cc7fc();
extern u8 data_02087ccc[];
extern u8 data_02087cb8[];
s32 func_ov001_022c8cc0();
s32 func_ov001_022c8d70();
void func_ov001_022cca00();

void func_ov001_022cc8ec(void) {
    s32 t0 = *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10);
    s32 t1 = (*(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x10) + 0x1ec);
    *(u32 *)((u8 *)t1 + 0x10) = 0;
    *(u32 *)((u8 *)t1 + 0x14) = 0;
    *(u32 *)((u8 *)t1 + 0x8) = 0;
    func_ov001_022cc17c(t0);
    func_ov001_022cbf30(t0);
    s32 t2 = (*(u32 *)(t0 + 0x1ec) + 0x1);
    *(u32 *)(t0 + 0x1ec) = t2;
    *(u32 *)((u8 *)t0 + 0x14) = (*(u32 *)((u8 *)t0 + 0x14) + 0x1);
    s32 t3 = (*(u32 *)((u8 *)t0 + 0x18) + 0x1);
    *(u32 *)((u8 *)t0 + 0x18) = t3;
    s32 t4 = func_02050018(t3, t2);
    if (t4 != 0) {
        s32 t5 = func_0205000c();
        if (t5 == 0) {
            *(u32 *)t1 = 0;
            *(u32 *)((u8 *)t0 + 0x14) = 0;
            *(u32 *)((u8 *)t0 + 0x18) = 0;
            *(u32 *)((u8 *)t1 + 0x4) = 4;
            *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov001_022cc7fc;
            *(u32 *)data_02087ccc = 1;
            s32 t6 = (*(u32 *)data_02087cb8 | 0x10);
            *(u32 *)data_02087cb8 = t6;
            func_ov001_022c8cc0(t1, data_02087cb8, t6);
            func_ov001_022c8d70(t1);
            return;
        }
        *(u32 *)((u8 *)t1 + 0x4) = 1;
        *(u32 *)((u8 *)*(u32 *)data_022b4574 + 0x8) = (u32)func_ov001_022cca00;
    }
}

#pragma thumb reset
