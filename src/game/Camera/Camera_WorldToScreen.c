#include <nitro/types.h>

void Camera_GetViewOrigin(void *a, s32 *x, s32 *y);

void Camera_WorldToScreen(void *a, s32 x, s32 y, s16 *ox, s16 *oy) {
    s32 bx, by;

    Camera_GetViewOrigin(a, &bx, &by);
    *ox = (x - bx) >> 8;
    *oy = (y - by) >> 8;
}
