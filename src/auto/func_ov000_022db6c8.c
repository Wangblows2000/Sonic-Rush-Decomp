/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

void func_ov000_022db6c8(u8 *p0) {
    s32 t0 = *(u32 *)p0;
    if (t0 < 0x4000) {
        *(u32 *)p0 = (t0 + 0x749fe);
        return;
    }
    if (t0 > 0x789fe) {
        *(u32 *)p0 = (t0 - 0x749fe);
    }
}
