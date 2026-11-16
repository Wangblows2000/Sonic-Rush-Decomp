#include <nitro/types.h>

extern u8 data_ov001_023178f4[];
u8 *func_ov001_0230c0a4(void);
s32 NNS_G3dGetResDictIdxByName(void *p, void *k);
void NNS_G3dGetCurrentMtx(void *p, s32 a);

void func_ov001_02311e64(u8 *p) {
    u8 *g = func_ov001_0230c0a4();
    s32 expect;
    if (*(u32 *)(p + 8) & 0x10) {
        expect = p[0xae];
    } else {
        expect = -1;
    }
    if (expect != NNS_G3dGetResDictIdxByName(*(u8 **)(*(u8 **)(p + 4) + 4) + 0x40, data_ov001_023178f4)) {
        return;
    }
    NNS_G3dGetCurrentMtx(g + 0x1f8, 0);
    *(s32 *)(p + 0x24) = 0;
    p[0x92] = 0;
}
