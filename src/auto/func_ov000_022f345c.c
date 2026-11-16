/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f9044();
s32 ObjRect_SetBox3D();
s32 func_ov000_022f4cbc();
extern u8 data_ov000_02369ac0[];
void func_ov000_022f33f8();

void func_ov000_022f345c(u8 *p0) {
    *(u16 *)((u8 *)p0 + 0x2e) = 0;
    *(u16 *)((u8 *)p0 + 0x436) = 0;
    func_ov000_022f9044(p0, ((u8 *)p0 + 0x400), 0);
    ObjRect_SetBox3D(((u8 *)p0 + 0x290), -120, -48, 0x10, 0x38, -0x7d00, 0x7d00);
    ObjRect_SetBox3D(((u8 *)p0 + 0x314), -20, -48, 0x74, 0x38, -0x7d00, 0x7d00);
    func_ov000_022f4cbc(p0);
    *(u32 *)((u8 *)data_ov000_02369ac0 + 0x4) = 0x31;
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f33f8;
}
