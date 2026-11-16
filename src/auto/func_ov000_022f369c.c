/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f4cbc();
extern u8 data_ov000_02363e48[];
void func_ov000_022f3bb8();
s32 func_ov000_022f4ce4();
void func_ov000_022f35e4();

void func_ov000_022f369c(u8 *p0) {
    func_ov000_022f4cbc();
    *(u16 *)((u8 *)p0 + 0x438) = (*(u16 *)((u8 *)p0 + 0x438) + 0x1);
    s32 t0 = *(u32 *)((u8 *)p0 + 0x458);
    s32 v1 = (s32)data_ov000_02363e48;
    if (*(u16 *)((u8 *)p0 + 0x438) < *(u8 *)((u8 *)data_ov000_02363e48 + *(u32 *)((u8 *)p0 + 0x458))) {
        if (t0 >= 6) {
            *(u32 *)((u8 *)p0 + 0x434) = (*(u32 *)((u8 *)p0 + 0x434) == 0);
            *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f3bb8;
            return;
        }
        v1 = ((*(u32 *)((u8 *)p0 + 0x434) != 0) ? 0xe : 0xf);
    } else {
        v1 = ((*(u32 *)((u8 *)p0 + 0x434) != 0) ? 0xc : 0xd);
    }
    func_ov000_022f4ce4(p0, v1, 0x1000, 0);
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022f35e4;
    ((s32 (*)())*(u32 *)((u8 *)p0 + 0x68))(p0, *(u32 *)((u8 *)p0 + 0x68));
}
