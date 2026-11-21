#include <nitro/types.h>

void Camera_SetLeadTarget(s32 a, s32 b, s32 c);

void Camera_SetLeadTargetPrimary(s32 a, s32 b) {
    Camera_SetLeadTarget(0, a, b);
}
