#include <nitro/types.h>

typedef struct {
    fx32 _00, _01, _10, _11;
} UnkMtx22_02000d98;

void func_02000d98(const UnkMtx22_02000d98 *pSrc, UnkMtx22_02000d98 *pDst, fx32 x, fx32 y)
{
    pDst->_00 = (fx32)(((s64)x * pSrc->_00) >> 12);
    pDst->_01 = (fx32)(((s64)x * pSrc->_01) >> 12);
    pDst->_10 = (fx32)(((s64)y * pSrc->_10) >> 12);
    pDst->_11 = (fx32)(((s64)y * pSrc->_11) >> 12);
}
