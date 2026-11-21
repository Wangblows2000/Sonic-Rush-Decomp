/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236a244[];
s32 ObjData_Release();
s32 ObjVram_Free();
s32 MapObject_Destructor();

void GmkSpecial_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    ObjData_Release(data_ov000_0236a244);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x240);
    if (t1 != 0) {
        ObjVram_Free(0, t1);
    }
    s32 t2 = *(u32 *)((u8 *)t0 + 0x288);
    if (t2 != 0) {
        ObjVram_Free(1, t2);
    }
    MapObject_Destructor(p0);
}
