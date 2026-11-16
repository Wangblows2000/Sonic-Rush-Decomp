/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 data_ov000_0236a988[];
extern u8 data_ov000_0236a990[];
s32 ObjData_Release();
s32 StageObj_Destructor();

void func_ov000_0232d5e4(s32 p0) {
    *(u32 *)data_ov000_0236a988 = 0;
    ObjData_Release(data_ov000_0236a990, data_ov000_0236a988, 0);
    ObjData_Release(data_ov000_0236a990);
    StageObj_Destructor(p0);
}
