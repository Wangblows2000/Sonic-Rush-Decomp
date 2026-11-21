#include <nitro/types.h>

typedef struct { u32 m[9]; } UnkMtx_020308cc;
extern u8 data_02084d84[];
void Model_TransposeRot(void *src, UnkMtx_020308cc *dst);
void NNS_G3dGeBufferOP_N(u32 a, void *b, u32 c);

void Model_MatrixOp_MultCameraRot(void) {
    UnkMtx_020308cc m;
    Model_TransposeRot(data_02084d84, &m);
    NNS_G3dGeBufferOP_N(0x1A, &m, 9);
}
