/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369e9c[];
s32 ObjData_Release();
extern u8 data_ov000_02369ea4[];
s32 ObjVramRef_Release();
extern u8 data_ov000_02369eac[];
s32 ObjDraw_ReleaseSpritePalette();
s32 MapObject_Destructor();

void CranePartED_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    ObjData_Release(data_ov000_02369e9c);
    ObjVramRef_Release(data_ov000_02369ea4, 0);
    ObjVramRef_Release(data_ov000_02369eac, 1);
    ObjDraw_ReleaseSpritePalette((*(u16 *)((u8 *)(t0 + 0x900) + 0xe0) & 0xff));
    MapObject_Destructor(p0);
}
