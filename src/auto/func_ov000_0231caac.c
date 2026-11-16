/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236a388[];
s32 ObjData_Release();
extern u8 data_ov000_0236a390[];
s32 ObjVramRef_Release();
extern u8 data_ov000_0236a398[];
extern u8 data_ov000_0236a3a0[];
extern u8 data_ov000_0236a3a8[];
s32 func_ov000_022fb9c8();

void func_ov000_0231caac(s32 p0) {
    ObjData_Release(data_ov000_0236a388);
    ObjVramRef_Release(data_ov000_0236a390, 0);
    ObjVramRef_Release(data_ov000_0236a398, 1);
    ObjData_Release(data_ov000_0236a388);
    ObjVramRef_Release(data_ov000_0236a3a0, 0);
    ObjVramRef_Release(data_ov000_0236a3a8, 1);
    func_ov000_022fb9c8(p0);
}
