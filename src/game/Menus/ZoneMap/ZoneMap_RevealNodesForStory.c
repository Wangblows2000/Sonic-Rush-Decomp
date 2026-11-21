#include <nitro/types.h>

void ZoneMap_RevealSonicNodes(void *p);
void ZoneMap_RevealBlazeNodes(void *p);

#pragma thumb on
void ZoneMap_RevealNodesForStory(s32 *p) {
    switch (*p) {
    case 0:
        ZoneMap_RevealSonicNodes(p);
        break;
    case 1:
        ZoneMap_RevealBlazeNodes(p);
        break;
    }
}
#pragma thumb reset
