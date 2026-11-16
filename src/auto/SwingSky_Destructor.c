/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236a26c[];
s32 ObjData_Release();
extern u8 data_ov000_0236a274[];
s32 ObjVramRef_Release();
extern u8 data_ov000_0236a27c[];
extern u8 data_ov000_0236a284[];
extern u8 data_ov000_0236a28c[];
extern u8 data_ov000_0236a294[];
extern u8 data_ov000_0236a29c[];
s32 ObjVram_Free();
s32 MapObject_Destructor();

void SwingSky_Destructor(u8 *p0) {
    s32 t0 = *(u32 *)((u8 *)p0 + 0x10);
    ObjData_Release(data_ov000_0236a26c);
    ObjData_Release(data_ov000_0236a26c);
    ObjVramRef_Release(data_ov000_0236a274, 0);
    ObjVramRef_Release(data_ov000_0236a27c, 1);
    ObjVramRef_Release(data_ov000_0236a284, 0);
    ObjVramRef_Release(data_ov000_0236a28c, 1);
    ObjData_Release(data_ov000_0236a26c);
    ObjVramRef_Release(data_ov000_0236a294, 0);
    ObjVramRef_Release(data_ov000_0236a29c, 1);
    ObjData_Release(data_ov000_0236a26c);
    s32 t1 = *(u32 *)((u8 *)t0 + 0x3f0);
    if (t1 != 0) {
        ObjVram_Free(0, t1);
    }
    s32 t2 = *(u32 *)((u8 *)t0 + 0x438);
    if (t2 != 0) {
        ObjVram_Free(1, t2);
    }
    MapObject_Destructor(p0);
}
