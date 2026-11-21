#include <nitro/types.h>

typedef struct {
    u32 flag;
    fx32 scale[3];
    fx32 invScale[3];
    fx32 unk1C[3];
    fx32 rot[9];
    fx32 trans[3];
} UnkJntAnm_020291fc;

void NNS_G3dGeBufferOP_N(u32 op, const void *args, u32 n);

#define FX_MUL_020291fc(a, b) ((fx32)(((s64)(a) * (b)) >> 12))

void NNSi_G3dSendJointSRTSi3d(UnkJntAnm_020291fc *r) {
    BOOL doTrans = FALSE;
    u32 ssc = r->flag & 0x18;
    if (!ssc) NNS_G3dGeBufferOP_N(0x1B, r->unk1C, 3);
    if (!(r->flag & 4)) {
        if (ssc) {
            doTrans = TRUE;
        } else {
            fx32 t[3];
            t[0] = FX_MUL_020291fc(r->trans[0], r->invScale[0]);
            t[1] = FX_MUL_020291fc(r->trans[1], r->invScale[1]);
            t[2] = FX_MUL_020291fc(r->trans[2], r->invScale[2]);
            NNS_G3dGeBufferOP_N(0x1C, t, 3);
        }
    }
    if (!(r->flag & 2)) {
        if (doTrans) {
            NNS_G3dGeBufferOP_N(0x19, r->rot, 0xC);
        } else {
            NNS_G3dGeBufferOP_N(0x1A, r->rot, 9);
        }
    } else if (doTrans) {
        NNS_G3dGeBufferOP_N(0x1C, r->trans, 3);
    }
    if (!ssc) NNS_G3dGeBufferOP_N(0x1B, r->invScale, 3);
    if (!(r->flag & 1)) NNS_G3dGeBufferOP_N(0x1B, r->scale, 3);
}
