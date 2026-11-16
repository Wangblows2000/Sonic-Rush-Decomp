#include <nitro/types.h>

void func_ov000_02342744(void *p);
void MapObject_Destructor(void *p);

void MapObj74_Destructor(void *p) {
    func_ov000_02342744(p);
    MapObject_Destructor(p);
}
