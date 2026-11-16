#include <nitro/types.h>

void Camera_RemoveAreaObject(void *p);
void MapObject_Destructor(void *p);

void MapObj73_Destructor(void *p) {
    Camera_RemoveAreaObject(p);
    MapObject_Destructor(p);
}
