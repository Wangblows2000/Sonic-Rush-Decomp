#include <nitro/types.h>

typedef struct {
    fx32 x, y, z;
} UnkVec_020044f4;

typedef struct {
    fx32 _[12];
} UnkMtx43_020044f4;

void VEC_Normalize(const UnkVec_020044f4 *src, UnkVec_020044f4 *dst);
void VEC_CrossProduct(const UnkVec_020044f4 *a, const UnkVec_020044f4 *b, UnkVec_020044f4 *ab);
fx32 VEC_DotProduct(const UnkVec_020044f4 *a, const UnkVec_020044f4 *b);

#define REG_MTX_MODE (*(vu32 *)0x04000440)
#define REG_MTX_LOAD_4x3 (*(vu32 *)0x0400045c)

void G3i_LookAt_(const UnkVec_020044f4 *camPos, const UnkVec_020044f4 *camUp, const UnkVec_020044f4 *target, BOOL draw, UnkMtx43_020044f4 *mtx)
{
    UnkVec_020044f4 vLook, vRight, vUp;
    fx32 t1, t2, t3;
    vu32 *load;

    vLook.x = camPos->x - target->x;
    vLook.y = camPos->y - target->y;
    vLook.z = camPos->z - target->z;
    VEC_Normalize(&vLook, &vLook);
    VEC_CrossProduct(camUp, &vLook, &vRight);
    VEC_Normalize(&vRight, &vRight);
    VEC_CrossProduct(&vLook, &vRight, &vUp);

    if (draw) {
        REG_MTX_MODE = 2;
        load = &REG_MTX_LOAD_4x3;
        *load = vRight.x;
        *load = vUp.x;
        *load = vLook.x;
        *load = vRight.y;
        *load = vUp.y;
        *load = vLook.y;
        *load = vRight.z;
        *load = vUp.z;
        *load = vLook.z;
    }
    t1 = -VEC_DotProduct(camPos, &vRight);
    t2 = -VEC_DotProduct(camPos, &vUp);
    t3 = -VEC_DotProduct(camPos, &vLook);
    if (draw) {
        *load = t1;
        *load = t2;
        *load = t3;
    }
    if (mtx != NULL) {
        mtx->_[0] = vRight.x;
        mtx->_[1] = vUp.x;
        mtx->_[2] = vLook.x;
        mtx->_[3] = vRight.y;
        mtx->_[4] = vUp.y;
        mtx->_[5] = vLook.y;
        mtx->_[6] = vRight.z;
        mtx->_[7] = vUp.z;
        mtx->_[8] = vLook.z;
        mtx->_[9] = t1;
        mtx->_[10] = t2;
        mtx->_[11] = t3;
    }
}
