/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236ac60[];
s32 func_ov000_022ec16c();
extern u8 data_022c4560[];
s32 func_ov000_022ec15c();
s32 func_ov000_02328430();
extern u8 data_0206f470[];
s32 MTX_RotY33_();
extern u8 data_02071470[];
s32 func_02036754();
void func_ov000_022ed78c();

void func_ov000_022ed8b4(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x214);
    s32 t1 = *(u32 *)data_ov000_0236ac60;
    s32 t2 = func_ov000_022ec16c(*(u32 *)((u8 *)p0 + 0x214), *(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x214) + 0x3d0), data_ov000_0236ac60);
    *(u32 *)((u8 *)p0 + 0x218) = t2;
    s32 t3 = func_ov000_022ec16c(t1, *(u32 *)data_022c4560);
    *(u32 *)((u8 *)p0 + 0x21c) = t3;
    func_ov000_022ec15c(*(u32 *)((u8 *)p0 + 0x218));
    func_ov000_022ec15c(*(u32 *)((u8 *)p0 + 0x21c));
    s32 t4 = func_ov000_02328430(1, *(u32 *)((u8 *)t1 + 0x10), *(u32 *)((u8 *)t1 + 0x14), *(u32 *)((u8 *)t1 + 0x18));
    MTX_RotY33_((t4 + 0x158), *(s16 *)data_0206f470, *(s16 *)((u8 *)data_0206f470 + 0x2));
    s32 t5 = func_ov000_02328430(1, *(u32 *)((u8 *)t0 + 0x10), *(u32 *)((u8 *)t0 + 0x14), *(u32 *)((u8 *)t0 + 0x18));
    MTX_RotY33_((t5 + 0x158), *(s16 *)data_02071470, *(s16 *)((u8 *)data_02071470 + 0x2));
    if (*(u8 *)((u8 *)t1 + 0x324) == 0) {
        func_02036754(0, 0, 0xa);
    } else {
        func_02036754(0, 0, 0x17);
    }
    *(u32 *)((u8 *)p0 + 0x68) = (u32)func_ov000_022ed78c;
}
