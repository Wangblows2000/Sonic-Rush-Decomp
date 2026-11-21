#include <nitro/types.h>

typedef struct { u32 flag; u8 unk4[0x2C]; fx32 magW; fx32 magH; } UnkTexMtx_020285ac;
typedef void (*UnkTexMtxFn_020285ac)(fx32 *m, UnkTexMtx_020285ac *tm);

extern UnkTexMtxFn_020285ac data_02077ce0[];

void NNS_G3dGeBufferOP_N(u32 op, const void *args, u32 n);

#define FX_MUL_020285ac(a, b) ((fx32)(((s64)(a) * (b)) >> 12))

void NNSi_G3dSendTexSRTMaya(UnkTexMtx_020285ac *tm) {
    struct {
        u32 cmd;
        u32 mode;
        fx32 m[16];
        u32 mode2;
    } buf;
    if (tm->flag & 8) { buf.cmd = 0x101610; } else { buf.cmd = 0x101810; }
    buf.mode = 3;
    buf.mode2 = 2;
    buf.m[14] = 0; buf.m[11] = 0; buf.m[10] = 0; buf.m[9] = 0; buf.m[8] = 0; buf.m[7] = 0; buf.m[6] = 0; buf.m[3] = 0; buf.m[2] = 0;
    buf.m[15] = 0x1000;
    data_02077ce0[tm->flag & 7](buf.m, tm);
    if (tm->magW != 0x1000) {
        buf.m[0] = FX_MUL_020285ac(tm->magW, buf.m[0]);
        buf.m[1] = FX_MUL_020285ac(tm->magW, buf.m[1]);
        buf.m[12] = FX_MUL_020285ac(tm->magW, buf.m[12]);
    }
    if (tm->magH != 0x1000) {
        buf.m[4] = FX_MUL_020285ac(tm->magH, buf.m[4]);
        buf.m[5] = FX_MUL_020285ac(tm->magH, buf.m[5]);
        buf.m[13] = FX_MUL_020285ac(tm->magH, buf.m[13]);
    }
    NNS_G3dGeBufferOP_N(buf.cmd, (u8 *)&buf + 4, 0x12);
}
