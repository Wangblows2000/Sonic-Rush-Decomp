#pragma opt_propagation off
#pragma opt_strength_reduction off
#include <nitro/types.h>

typedef struct { u32 flags; u8 pad[0x60]; } UnkStruct_02339c2c;

extern UnkStruct_02339c2c gCamera[];
extern UnkStruct_02339c2c data_ov000_0236ab3c;
void Camera_RunLookCallback(void *a);
void Camera_Unk_ov000_02342834(u16 i);
void Camera_UpdateX(void *a, s32 i);
void Camera_ClampXToMapBounds(void *a, s32 i);
void Camera_UpdateY(void *a, s32 i);
void Camera_ClampYToMapBounds(void *a, s32 i);
void Camera_ClampXAroundTarget(void *a, s32 i);
void Camera_UpdateYDualScreen(void *a, s32 i);
void Camera_ClampYAroundTarget(void *a, s32 i);
void Camera_Unk_ov000_02339094(void *a, s32 i);
void Camera_Unk_ov000_0233905c(void *a, s32 i);

void Camera_UpdateFollow(void *a) {
    UnkStruct_02339c2c *base;
    UnkStruct_02339c2c *q;
    UnkStruct_02339c2c *p;
    s32 i;
    if ((gCamera[2].flags & 0x100) == 0)
        Camera_RunLookCallback(a);
    base = gCamera;
    if (base[2].flags & 0x200)
        return;
    if (base[2].flags & 1) {
        if ((base[0].flags & 0x140) == 0) {
            Camera_UpdateX(a, 0);
            Camera_ClampXAroundTarget(a, 0);
        } else {
            Camera_ClampXToMapBounds(a, 0);
        }
        if ((base[0].flags & 0x280) == 0) {
            Camera_UpdateYDualScreen(a, 0);
            Camera_ClampYAroundTarget(a, 0);
        } else {
            Camera_ClampYToMapBounds(a, 0);
        }
        q = &data_ov000_0236ab3c;
        if ((q->flags & 0x140) == 0)
            Camera_Unk_ov000_02339094(a, 1);
        if ((q->flags & 0x280) == 0)
            Camera_Unk_ov000_0233905c(a, 1);
        return;
    }
    i = 0;
    while (i < 2) {
        p = &base[i];
        Camera_Unk_ov000_02342834(i);
        if ((base[i].flags & 0x140) == 0) Camera_UpdateX(a, i); else Camera_ClampXToMapBounds(a, i);
        if ((p->flags & 0x280) == 0) Camera_UpdateY(a, i); else Camera_ClampYToMapBounds(a, i);
        i++;
    }
}
