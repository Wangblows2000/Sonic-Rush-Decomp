/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_02369e64[];
s32 ObjData_Release();
extern u8 data_ov000_02369e74[];
s32 ObjVramRef_Release();
extern u8 data_ov000_02369e7c[];
extern u8 data_ov000_02369e84[];
extern u8 data_ov000_02369e8c[];
extern u8 data_ov000_02369e6c[];
s32 MapObject_Destructor();

void Cargo_Destructor(s32 p0) {
    ObjData_Release(data_ov000_02369e64);
    ObjVramRef_Release(data_ov000_02369e74, 0);
    ObjVramRef_Release(data_ov000_02369e7c, 1);
    ObjData_Release(data_ov000_02369e64);
    ObjVramRef_Release(data_ov000_02369e84, 0);
    ObjVramRef_Release(data_ov000_02369e8c, 1);
    ObjData_Release(data_ov000_02369e6c);
    MapObject_Destructor(p0);
}
