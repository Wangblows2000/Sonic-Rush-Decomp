#include <nitro/types.h>

void MapObject_Destructor(void);

void EnemyPawnGroup_Destructor(void) {
    MapObject_Destructor();
}
