#include <nitro/types.h>

void MapObject_Destructor(void);

void Boss4PartCE_Destructor(void) {
    MapObject_Destructor();
}
