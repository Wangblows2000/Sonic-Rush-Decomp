/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov001_0231d6b4[];
extern u8 data_ov001_0231f098[];
s32 NNS_G3dResDefaultRelease();

void func_ov001_022fa574(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x1c8);
    if ((*(u32 *)((u8 *)data_ov001_0231d6b4 + (*(u32 *)((u8 *)p0 + 0x1c8) * 0x1c)) & 1) != 0) {
        return;
    }
    NNS_G3dResDefaultRelease(*(u32 *)((u8 *)data_ov001_0231f098 + (t0 * 0x18)), (t0 * 0x18), t0);
}
