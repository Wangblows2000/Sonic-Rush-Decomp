#include <nitro/types.h>

void MapObject_Destructor(void);

void WaterLevelTrigger_Destructor(void) {
    MapObject_Destructor();
}
