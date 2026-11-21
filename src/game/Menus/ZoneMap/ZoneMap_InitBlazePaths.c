#include <nitro/types.h>

void ZoneMap_InitPath(void *p, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g);
void ZoneMap_ClearPath(void *p, s32 i);

#pragma thumb on
void ZoneMap_InitBlazePaths(void *p, s32 a) {
    ZoneMap_InitPath(p, a, 0, 35, 36, 1, 11, 5);
    ZoneMap_InitPath(p, a, 1, 37, 38, 16, 12, 7);
    ZoneMap_InitPath(p, a, 16, 39, 40, 2, 13, 7);
    ZoneMap_InitPath(p, a, 2, 41, 42, 3, 14, 5);
    ZoneMap_InitPath(p, a, 3, 43, 44, 4, 15, 5);
    ZoneMap_InitPath(p, a, 4, 45, 46, 6, 16, 5);
    ZoneMap_InitPath(p, a, 5, 47, 48, 8, 17, 5);
    ZoneMap_InitPath(p, a, 8, 49, 50, 10, 18, 5);
    ZoneMap_InitPath(p, a, 10, 51, 52, 12, 19, 5);
    ZoneMap_InitPath(p, a, 11, 53, 54, 13, 20, 3);
    ZoneMap_InitPath(p, a, 13, 55, 56, 14, 21, 5);
    ZoneMap_ClearPath(p, 0);
    ZoneMap_ClearPath(p, 1);
    ZoneMap_ClearPath(p, 2);
    ZoneMap_ClearPath(p, 3);
    ZoneMap_ClearPath(p, 4);
    ZoneMap_ClearPath(p, 5);
    ZoneMap_ClearPath(p, 6);
    ZoneMap_ClearPath(p, 7);
    ZoneMap_ClearPath(p, 8);
    ZoneMap_ClearPath(p, 9);
    ZoneMap_ClearPath(p, 10);
}
#pragma thumb reset
