#include <nitro/types.h>

typedef struct { u32 flag; u8 unk4[0x14]; fx32 scaleS; fx32 scaleT; u8 unk20[4]; fx32 transS; fx32 transT; u16 origW; u16 origH; fx32 magW; fx32 magH; } UnkTexMtx_02028dfc;

void func_02025268(u32 op, const void *args, u32 n);

#define FX_MUL_02028dfc(a, b) ((fx32)(((s64)(a) * (b)) >> 12))

void func_02028dfc(UnkTexMtx_02028dfc *tm) {
    struct {
        u32 cmd;
        u32 mode;
        fx32 m[12];
        u32 mode2;
    } buf;
    if (tm->flag & 8) { buf.cmd = 0x101710; } else { buf.cmd = 0x101910; }
    buf.mode = 3;
    buf.mode2 = 2;
    buf.m[11] = 0; buf.m[8] = 0; buf.m[7] = 0; buf.m[6] = 0; buf.m[5] = 0; buf.m[3] = 0; buf.m[2] = 0; buf.m[1] = 0;
    if (tm->flag & 4) {
        buf.m[9] = 0;
        buf.m[10] = 0;
        if (tm->flag & 1) {
            buf.m[0] = 0x1000;
            buf.m[4] = 0x1000;
        } else {
            buf.m[0] = tm->scaleS;
            buf.m[4] = tm->scaleT;
        }
    } else if (tm->flag & 1) {
        buf.m[9] = -(tm->transS << 4) * tm->origW;
        buf.m[10] = -(tm->transT << 4) * tm->origH;
        buf.m[0] = 0x1000;
        buf.m[4] = 0x1000;
    } else {
        buf.m[9] = -(fx32)(((s64)tm->scaleS * tm->transS) >> 8) * tm->origW;
        buf.m[10] = -(fx32)(((s64)tm->scaleT * tm->transT) >> 8) * tm->origH;
        buf.m[0] = tm->scaleS;
        buf.m[4] = tm->scaleT;
    }
    if (tm->magW != 0x1000) {
        buf.m[0] = FX_MUL_02028dfc(tm->magW, buf.m[0]);
        buf.m[9] = FX_MUL_02028dfc(tm->magW, buf.m[9]);
    }
    if (tm->magH != 0x1000) {
        buf.m[4] = FX_MUL_02028dfc(tm->magH, buf.m[4]);
        buf.m[10] = FX_MUL_02028dfc(tm->magH, buf.m[10]);
    }
    func_02025268(buf.cmd, (u8 *)&buf + 4, 0xE);
}
