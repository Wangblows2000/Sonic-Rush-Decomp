#include <nitro/types.h>
#include <main_a2/g3drs.h>

typedef struct { fx32 m[4][4]; } UnkMtx44_020238e8;
typedef struct { fx32 x, y, z; } UnkVec_020238e8;
typedef struct { fx32 m[4][3]; } UnkMtx43_020238e8;
typedef struct { u8 unk0[0xFC]; u32 flag; } UnkGlb_020238e8;

extern UnkVec_020238e8 data_02077c48;
extern UnkVec_020238e8 data_02077c54;
extern UnkMtx43_020238e8 data_02077c24;
extern u32 data_02077c1c[];
extern u32 data_02077c18[];
extern UnkGlb_020238e8 NNS_G3dGlb;
extern u8 data_02084d84[];

void NNS_G3dGeBufferOP_N(u32 op, const void *args, u32 n);
void NNS_G3dGeFlushBuffer(void);
BOOL G3X_GetClipMtx(UnkMtx44_020238e8 *m);
void *NNS_G3dGlbGetWV(void);
void *NNS_G3dGlbGetInvWV(void);
void *NNS_G3dGlbGetInvV(void);
void MTX_Copy43To44_(const void *src, UnkMtx44_020238e8 *dst);
void MTX_Concat44(const UnkMtx44_020238e8 *a, const UnkMtx44_020238e8 *b, UnkMtx44_020238e8 *dst);
fx32 VEC_Mag(const void *v);
void VEC_Normalize(const void *src, void *dst);
void MIi_CpuSend32(const void *src, volatile void *dst, u32 size);

#define REG_GXFIFO (*(vu32 *)0x04000400)

void func_020238e8(UnkRS *rs, u32 opt) {
    int t;
    u32 len;
    UnkVec_020238e8 *trans;
    UnkVec_020238e8 *scale;
    UnkMtx43_020238e8 *mtx;
    BOOL skip;
    len = 2;
    trans = &data_02077c48;
    scale = &data_02077c54;
    mtx = &data_02077c24;
    if (rs->flag & 0x200) {
        if (opt == 0x40 || opt == 0x60) len++;
        if (opt == 0x20 || opt == 0x60) len++;
        rs->c += len;
        return;
    }
    if (opt == 0x40 || opt == 0x60) {
        len++;
        if (!(rs->flag & 0x100)) {
            u32 arg;
            if (opt == 0x40) {
                arg = rs->c[2];
            } else {
                arg = rs->c[3];
            }
            NNS_G3dGeBufferOP_N(0x14, &arg, 1);
        }
    }
    skip = UnkRS_CbA(rs, 8, &t);
    if (!(rs->flag & 0x100) && !skip) {
        UnkMtx44_020238e8 m;
        NNS_G3dGeFlushBuffer();
        REG_GXFIFO = 0x151110;
        REG_GXFIFO = 0;
        REG_GXFIFO = 0;
        while (G3X_GetClipMtx(&m)) {
        }
        if (NNS_G3dGlb.flag & 1) {
            UnkMtx44_020238e8 tmp;
            MTX_Copy43To44_(NNS_G3dGlbGetWV(), &tmp);
            MTX_Concat44(&m, &tmp, &m);
        } else if (NNS_G3dGlb.flag & 2) {
            UnkMtx44_020238e8 tmp;
            MTX_Copy43To44_(data_02084d84, &tmp);
            MTX_Concat44(&m, &tmp, &m);
        }
        trans->x = m.m[3][0];
        trans->y = m.m[3][1];
        trans->z = m.m[3][2];
        scale->x = VEC_Mag(m.m[0]);
        scale->y = VEC_Mag(m.m[1]);
        scale->z = VEC_Mag(m.m[2]);
        if (m.m[1][1] != 0 || m.m[1][2] != 0) {
            VEC_Normalize(m.m[1], mtx->m[1]);
            mtx->m[2][1] = -mtx->m[1][2];
            mtx->m[2][2] = mtx->m[1][1];
        } else {
            VEC_Normalize(m.m[2], mtx->m[2]);
            mtx->m[1][2] = -mtx->m[2][1];
            mtx->m[1][1] = mtx->m[2][2];
        }
        if (NNS_G3dGlb.flag & 1) {
            REG_GXFIFO = 0x171012;
            MIi_CpuSend32(data_02077c1c, &REG_GXFIFO, 8);
            MIi_CpuSend32(NNS_G3dGlbGetInvWV(), &REG_GXFIFO, 0x30);
            REG_GXFIFO = 0x1B19;
            MIi_CpuSend32(&data_02077c24, &REG_GXFIFO, 0x3C);
        } else if (NNS_G3dGlb.flag & 2) {
            REG_GXFIFO = 0x171012;
            MIi_CpuSend32(data_02077c1c, &REG_GXFIFO, 8);
            MIi_CpuSend32(NNS_G3dGlbGetInvV(), &REG_GXFIFO, 0x30);
            REG_GXFIFO = 0x1B19;
            MIi_CpuSend32(&data_02077c24, &REG_GXFIFO, 0x3C);
        } else {
            MIi_CpuSend32(data_02077c18, &REG_GXFIFO, 0x48);
        }
    }
    if (t == 3) {
        rs->flag &= ~0x40;
        rs->cb[8](rs);
        skip = rs->flag & 0x40;
    } else {
        skip = FALSE;
    }
    if (opt == 0x20 || opt == 0x60) {
        len++;
        if (!skip && !(rs->flag & 0x100)) {
            u32 arg2 = rs->c[2];
            NNS_G3dGeBufferOP_N(0x13, &arg2, 1);
        }
    }
    rs->c += len;
}
