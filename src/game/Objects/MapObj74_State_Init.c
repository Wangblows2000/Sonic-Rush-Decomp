/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Camera_CheckPosInView();
s32 MapObj74_IsPlayerInArea();
s32 MapObj74_Action_LockCamera();
s32 MapObj74_Action_ReleaseCamera();

void MapObj74_State_Init(u8 *p0) {
    Camera_CheckPosInView(*(u32 *)((u8 *)p0 + 0x10), *(u32 *)((u8 *)p0 + 0x14));
    s32 t0 = MapObj74_IsPlayerInArea(p0);
    if (t0 != 0) {
        MapObj74_Action_LockCamera(p0);
        return;
    }
    MapObj74_Action_ReleaseCamera(p0);
}
