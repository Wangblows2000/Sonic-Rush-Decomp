#include <nitro/types.h>

s32 NNS_G3dGetResDictIdxByName(void *p, s32 k);
void NNS_G3dGeBufferOP_N(s32 a, void *p, s32 n);

BOOL Boss6_StoreNodeMatrix(u8 *p, u32 v, s32 k) {
    s32 r = NNS_G3dGetResDictIdxByName(*(u8 **)(*(u8 **)(p + 4) + 4) + 0x40, k);
    s32 expect;
    if (*(u32 *)(p + 8) & 0x10) {
        expect = p[0xae];
    } else {
        expect = -1;
    }
    if (r != expect) {
        return FALSE;
    }
    {
        u32 tmp = v;
        NNS_G3dGeBufferOP_N(0x13, &tmp, 1);
    }
    return TRUE;
}
