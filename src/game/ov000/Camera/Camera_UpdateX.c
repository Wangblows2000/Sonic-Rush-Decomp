/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gCamera[];
s32 Camera_FollowPlayerX();
s32 Camera_ClampXToBounds();

void Camera_UpdateX(s32 p0, s32 p1) {
    s32 t0 = (*(u32 *)((u8 *)gCamera + ((s32)p1 * 0x64)) & 0x4010);
    s32 t1 = *(u32 *)((u8 *)gCamera + ((s32)p1 * 0x64));
    if (t0 == 0) {
        Camera_FollowPlayerX(p0, p1, t0, t1);
    }
    Camera_ClampXToBounds(p0, p1);
}
