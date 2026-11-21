#include <nitro/types.h>

typedef struct {
    u32 x0;
    u32 x4;
    u32 x8;
    u8 *xc;
    u8 *x10;
    u32 x14;
    u16 h18;
    u16 h1a;
    u32 x1c;
    u32 x20;
    u32 x24;
    u32 x28;
} UnkMap_0204bab0;

typedef struct {
    u8 pad0[0x124];
    u32 x124;
    u32 x128;
    u32 x12c;
    u32 x130;
    u16 *x134;
    u8 *x138;
} UnkGfx_0204bab0;

extern UnkGfx_0204bab0 gCamera;
extern UnkMap_0204bab0 gStageMapCollision;

void MapCollision_SetMap(void);

void Stage_InitMapCollision(void) {
    gStageMapCollision.x0 = gCamera.x124;
    gStageMapCollision.x4 = gCamera.x12c;
    gStageMapCollision.x8 = gCamera.x130;
    gStageMapCollision.xc = (u8 *)gCamera.x134 + 4;
    gStageMapCollision.x10 = gCamera.x138 + 4;
    gStageMapCollision.x14 = gCamera.x128;
    gStageMapCollision.h18 = gCamera.x134[0];
    gStageMapCollision.h1a = gCamera.x134[1];
    gStageMapCollision.x1c = 0;
    gStageMapCollision.x20 = 0;
    gStageMapCollision.x24 = gStageMapCollision.h18 << 6;
    gStageMapCollision.x28 = gStageMapCollision.h1a << 6;
    MapCollision_SetMap();
}
