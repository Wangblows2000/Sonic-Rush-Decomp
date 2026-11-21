#include <nitro/types.h>

typedef struct {
    fx32 _00, _01, _10, _11;
} UnkMtx22_02003d94;

void G2x_SetBGyAffine_(vu32 *addr, const UnkMtx22_02003d94 *mtx, s32 centerX, s32 centerY, s32 x1, s32 y1)
{
    s32 dx, dy;
    fx32 x2, y2;

    addr[0] = (u32)((u16)(fx16)(mtx->_00 >> 4) | (u16)(fx16)(mtx->_01 >> 4) << 16);
    addr[1] = (u32)((u16)(fx16)(mtx->_10 >> 4) | (u16)(fx16)(mtx->_11 >> 4) << 16);
    dx = x1 - centerX;
    dy = y1 - centerY;
    x2 = mtx->_00 * dx + mtx->_01 * dy + (centerX << 12);
    y2 = mtx->_10 * dx + mtx->_11 * dy + (centerY << 12);
    addr[2] = (u32)(x2 >> 4);
    addr[3] = (u32)(y2 >> 4);
}
