/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gExtraBossWork[];
void func_ov001_022fef68();
void func_ov001_022fef1c();
void func_ov001_022feed8();
extern u8 data_ov001_0231f520[];
extern u8 data_ov001_0231f524[];
s32 func_ov001_023063a0();
s32 func_ov001_023010e0();
s32 func_ov001_0230109c();
s32 func_ov001_02303c54();
s32 FX_ModS32();
s32 func_ov001_02300128();
s32 func_ov001_022f71b8();

void func_ov001_022ffd2c(void) {
    *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gExtraBossWork + 0x8) + 0x8) = (u32)func_ov001_022fef68;
    *(u32 *)((u8 *)*(u32 *)((u8 *)*(u32 *)gExtraBossWork + 0xc) + 0x8) = (u32)func_ov001_022fef1c;
    s32 t0 = *(u32 *)((u8 *)*(u32 *)gExtraBossWork + 0x10);
    *(u32 *)((u8 *)t0 + 0x8) = (u32)func_ov001_022feed8;
    *(u32 *)data_ov001_0231f520 = 0;
    *(u32 *)data_ov001_0231f524 = 0;
    func_ov001_023063a0(data_ov001_0231f524, data_ov001_0231f520, 0, t0);
    s32 t1 = (*(u32 *)*(u32 *)gExtraBossWork & ~4);
    s32 t2 = *(u32 *)gExtraBossWork;
    *(u32 *)t2 = t1;
    func_ov001_023010e0(t1, t2);
    func_ov001_0230109c();
    func_ov001_02303c54();
    s32 t3 = FX_ModS32(*(u32 *)((u8 *)*(u32 *)gExtraBossWork + 0x40), 2);
    if (t3 == 0) {
        func_ov001_02300128(1);
    } else {
        func_ov001_02300128(0);
    }
    func_ov001_022f71b8(1);
    func_ov001_022f71b8(2);
}
