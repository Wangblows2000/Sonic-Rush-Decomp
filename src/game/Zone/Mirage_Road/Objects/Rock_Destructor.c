#include <nitro/types.h>

void MapObject_Destructor(void);

void Rock_Destructor(void) {
    MapObject_Destructor();
}
