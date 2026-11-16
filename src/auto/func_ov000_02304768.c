/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369e5c[];
s32 ObjData_Release();
s32 ObjVram_Free();
s32 func_ov000_022fb9c8();

void func_ov000_02304768(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    ObjData_Release(data_ov000_02369e5c);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x2d0);
    if (t1 != 0) {
        ObjVram_Free(0, t1);
    }
    s32 t2 = *(u32 *)((u8 *)t0 + 0x318);
    if (t2 != 0) {
        ObjVram_Free(1, t2);
    }
    ObjData_Release(data_ov000_02369e5c);
    s32 t3 = *(u32 *)((u8 *)t0 + 0x240);
    if (t3 != 0) {
        ObjVram_Free(0, t3);
    }
    s32 t4 = *(u32 *)((u8 *)t0 + 0x288);
    if (t4 != 0) {
        ObjVram_Free(1, t4);
    }
    func_ov000_022fb9c8(p0);
}
