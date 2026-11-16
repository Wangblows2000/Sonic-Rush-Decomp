/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 ObjVramRef_Release();
s32 ObjVram_Free();
s32 func_ov000_022fb9c8();

void func_ov000_02306e90(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    if ((*(u32 *)((u8 *)*(u32 *)((u8 *)p0 + 0x10) + 0x2a0) & 1) == 0) {
        ObjVramRef_Release(*(u32 *)((u8 *)t0 + 0x29c), 0);
        ObjVramRef_Release((*(u32 *)((u8 *)t0 + 0x29c) + 0x8), 1);
    } else {
        s32 t1 = *(u32 *)((u8 *)t0 + 0x240);
        if (t1 != 0) {
            ObjVram_Free(0, t1);
        }
        s32 t2 = *(u32 *)((u8 *)t0 + 0x288);
        if (t2 != 0) {
            ObjVram_Free(1, t2);
        }
    }
    func_ov000_022fb9c8(p0);
}
