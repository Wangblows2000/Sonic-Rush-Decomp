#include <nitro/types.h>

typedef struct { u32 flag; u8 unk4[0x14]; fx32 scaleS; fx32 scaleT; fx16 sinR; fx16 cosR; fx32 transS; fx32 transT; u8 unk2C[4]; fx32 magW; fx32 magH; } UnkTexMtx_020299bc;
typedef void (*UnkTexMtxFn_020299bc)(fx32 *m, UnkTexMtx_020299bc *tm);

extern UnkTexMtxFn_020299bc data_02077d20[];

void NNS_G3dGeBufferOP_N(u32 op, const void *args, u32 n);

#define FX_MUL_020299bc(a, b) ((fx32)(((s64)(a) * (b)) >> 12))

void func_020299bc(UnkTexMtx_020299bc *tm) {
    struct {
        u32 cmd;
        u32 mode;
        fx32 m[16];
        u32 mode2;
    } buf;
    if (tm->flag & 8) { buf.cmd = 0x101610; } else { buf.cmd = 0x101810; }
    buf.mode = 3;
    buf.mode2 = 2;
    buf.m[15] = 0x1000;
    buf.m[14] = 0; buf.m[11] = 0; buf.m[10] = 0; buf.m[9] = 0; buf.m[8] = 0; buf.m[7] = 0; buf.m[6] = 0; buf.m[3] = 0; buf.m[2] = 0;
    if (tm->flag & 1) tm->scaleS = tm->scaleT = 0x1000;
    if (tm->flag & 2) {
        tm->cosR = 0x1000;
        tm->sinR = 0;
    }
    if (tm->flag & 4) tm->transS = tm->transT = 0;
    data_02077d20[tm->flag & 7](buf.m, tm);
    if (tm->magW != 0x1000) {
        buf.m[0] = FX_MUL_020299bc(tm->magW, buf.m[0]);
        buf.m[1] = FX_MUL_020299bc(tm->magW, buf.m[1]);
        buf.m[12] = FX_MUL_020299bc(tm->magW, buf.m[12]);
    }
    if (tm->magH != 0x1000) {
        buf.m[4] = FX_MUL_020299bc(tm->magH, buf.m[4]);
        buf.m[5] = FX_MUL_020299bc(tm->magH, buf.m[5]);
        buf.m[13] = FX_MUL_020299bc(tm->magH, buf.m[13]);
    }
    NNS_G3dGeBufferOP_N(buf.cmd, (u8 *)&buf + 4, 0x12);
}
