#include <nitro/types.h>

s32 ZoneMap_IsBlaze(void *a, void *b);
void ZoneMap_InitSonicNodes(void *a, void *b);
void ZoneMap_InitSonicPaths(void *a, void *b);
void ZoneMap_InitBlazeNodes(void *a, void *b);
void ZoneMap_InitBlazePaths(void *a, void *b);
void ZoneMap_Unk_ov001_022e432c(void *a, void *b);

#pragma thumb on
void ZoneMap_InitNodesAndPaths(void *a, void *b) {
    switch (ZoneMap_IsBlaze(a, b)) {
    case 0:
        ZoneMap_InitSonicNodes(a, b);
        ZoneMap_InitSonicPaths(a, b);
        break;
    case 1:
        ZoneMap_InitBlazeNodes(a, b);
        ZoneMap_InitBlazePaths(a, b);
        break;
    }
    ZoneMap_Unk_ov001_022e432c(a, b);
}
#pragma thumb reset
