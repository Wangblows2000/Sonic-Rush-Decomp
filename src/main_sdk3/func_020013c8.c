#include <nitro/types.h>

typedef struct {
    fx32 _[9];
} UnkMtx33_020013c8;

void func_020013c8(const UnkMtx33_020013c8 *pSrc, UnkMtx33_020013c8 *pDst, fx32 x, fx32 y, fx32 z)
{
    pDst->_[0] = (fx32)(((s64)x * pSrc->_[0]) >> 12);
    pDst->_[1] = (fx32)(((s64)x * pSrc->_[1]) >> 12);
    pDst->_[2] = (fx32)(((s64)x * pSrc->_[2]) >> 12);
    pDst->_[3] = (fx32)(((s64)y * pSrc->_[3]) >> 12);
    pDst->_[4] = (fx32)(((s64)y * pSrc->_[4]) >> 12);
    pDst->_[5] = (fx32)(((s64)y * pSrc->_[5]) >> 12);
    pDst->_[6] = (fx32)(((s64)z * pSrc->_[6]) >> 12);
    pDst->_[7] = (fx32)(((s64)z * pSrc->_[7]) >> 12);
    pDst->_[8] = (fx32)(((s64)z * pSrc->_[8]) >> 12);
}
