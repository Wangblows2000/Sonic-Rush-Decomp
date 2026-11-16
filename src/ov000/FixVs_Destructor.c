#include <nitro/types.h>

extern u8 data_ov000_0236a9a8;

void ObjData_Release(void *p);
void StageObj_Destructor(void *p);

void FixVs_Destructor(void *p) {
    ObjData_Release(&data_ov000_0236a9a8);
    StageObj_Destructor(p);
}
