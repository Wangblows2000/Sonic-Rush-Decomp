#include <nitro/types.h>

#define REG_DISPCNT (*(vu32 *)0x04000000)
#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

typedef struct {
    u8 pad0[0x28];
    s32 screen;
    u8 pad2c[0x10];
    u32 planes;
} UnkObj_0204f2fc;

static inline u32 GX_GetVisiblePlane(void) {
    return (REG_DISPCNT & 0x1f00) >> 8;
}

static inline void GX_SetVisiblePlane(u32 plane) {
    REG_DISPCNT = (REG_DISPCNT & ~0x1f00) | (plane << 8);
}

static inline u32 GXS_GetVisiblePlane(void) {
    return (REG_DB_DISPCNT & 0x1f00) >> 8;
}

static inline void GXS_SetVisiblePlane(u32 plane) {
    REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | (plane << 8);
}

#pragma thumb on
void func_0204f2fc(UnkObj_0204f2fc *o, BOOL show) {
    switch (o->screen) {
    case 0:
        if (show)
            GX_SetVisiblePlane(GX_GetVisiblePlane() | o->planes);
        else
            GX_SetVisiblePlane(GX_GetVisiblePlane() & ~o->planes);
        break;
    case 1:
        if (show)
            GXS_SetVisiblePlane(GXS_GetVisiblePlane() | o->planes);
        else
            GXS_SetVisiblePlane(GXS_GetVisiblePlane() & ~o->planes);
        break;
    }
}
#pragma thumb reset
