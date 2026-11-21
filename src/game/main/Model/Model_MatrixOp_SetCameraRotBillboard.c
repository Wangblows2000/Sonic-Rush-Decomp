#include <nitro/types.h>

typedef struct { u32 m[9]; } UnkMtx_02030808;
typedef struct { u8 unk0[0xFC]; u32 unkFC; } UnkStruct_02030808;
extern u8 data_02084d84[];
extern UnkMtx_02030808 data_02084df4;
extern UnkStruct_02030808 NNS_G3dGlb;
void Model_TransposeRotBillboard(void *src, UnkMtx_02030808 *dst);
void MI_Copy36B(const void *src, void *dst);

void Model_MatrixOp_SetCameraRotBillboard(void) {
    UnkMtx_02030808 m;
    Model_TransposeRotBillboard(data_02084d84, &m);
    MI_Copy36B(&m, &data_02084df4);
    NNS_G3dGlb.unkFC &= ~0xA4;
}
