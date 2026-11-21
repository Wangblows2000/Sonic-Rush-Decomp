#include <nitro/types.h>

extern u8 data_ov000_0236a024;

void ObjData_Release(void *p);
void MapObject_Destructor(void *p);

void JetCatapult_Destructor(void *p) {
    ObjData_Release(&data_ov000_0236a024);
    MapObject_Destructor(p);
}
