/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 Camera_CheckPosInView();
s32 MapObj74_IsPlayerInArea();
s32 MapObj74_Action_ReleaseCamera();
s32 Camera_GetPrimary();
s32 Camera_GetSecondary();
extern u8 gCamera[];

void MapObj74_State_CameraLocked(u8 *p0) {
    Camera_CheckPosInView(*(u32 *)((u8 *)p0 + 0x10), *(u32 *)((u8 *)p0 + 0x14));
    s32 t0 = MapObj74_IsPlayerInArea(p0);
    if (t0 == 0) {
        MapObj74_Action_ReleaseCamera(p0);
    }
    s32 t1 = Camera_GetPrimary();
    s32 t2 = Camera_GetSecondary();
    *(u32 *)((u8 *)t1 + 0x4) = (*(u32 *)((u8 *)p0 + 0x10) - 0x8000);
    *(u32 *)((u8 *)t1 + 0x8) = (*(u32 *)((u8 *)p0 + 0x14) - 0x6000);
    *(u32 *)((u8 *)t2 + 0x4) = (*(u32 *)((u8 *)t1 + 0x4) + *(u32 *)((u8 *)gCamera + 0xd4));
    *(u32 *)((u8 *)t2 + 0x8) = (*(u32 *)((u8 *)t1 + 0x8) + *(u32 *)((u8 *)gCamera + 0xd8));
}
