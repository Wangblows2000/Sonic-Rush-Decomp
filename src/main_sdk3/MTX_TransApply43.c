#include <nitro/types.h>

typedef struct {
    fx32 _00, _01, _02;
    fx32 _10, _11, _12;
    fx32 _20, _21, _22;
    fx32 _30, _31, _32;
} UnkMtx43_02001c00;

void MI_Copy36B(const void *src, void *dst);

void MTX_TransApply43(const UnkMtx43_02001c00 *pSrc, UnkMtx43_02001c00 *pDst, fx32 x, fx32 y, fx32 z)
{
    if (pSrc != pDst) {
        MI_Copy36B(pSrc, pDst);
    }
    pDst->_30 = (fx32)(((s64)x * pSrc->_00 + (s64)y * pSrc->_10 + (s64)z * pSrc->_20) >> 12) + pSrc->_30;
    pDst->_31 = (fx32)(((s64)x * pSrc->_01 + (s64)y * pSrc->_11 + (s64)z * pSrc->_21) >> 12) + pSrc->_31;
    pDst->_32 = (fx32)(((s64)x * pSrc->_02 + (s64)y * pSrc->_12 + (s64)z * pSrc->_22) >> 12) + pSrc->_32;
}
