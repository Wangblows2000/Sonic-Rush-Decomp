#include <nitro/types.h>

typedef struct { u32 m[9]; } UnkMtx_02030898;
extern u8 data_02084d84[];
void Model_TransposeRotBillboard(void *src, UnkMtx_02030898 *dst);
void NNS_G3dGeBufferOP_N(u32 a, void *b, u32 c);

void Model_MatrixOp_MultCameraRotBillboard(void) {
    UnkMtx_02030898 m;
    Model_TransposeRotBillboard(data_02084d84, &m);
    NNS_G3dGeBufferOP_N(0x1A, &m, 9);
}
