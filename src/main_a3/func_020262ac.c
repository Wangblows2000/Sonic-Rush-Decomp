#include <nitro/types.h>

typedef struct {
    u8 a;
    u8 b;
    u8 c;
    u8 d;
} UnkPivot_020262ac;

extern UnkPivot_020262ac data_02072b44[];

BOOL func_020262ac(fx32 *m, const s16 *rot3, const s16 *rot5, u32 info)
{
    if (info & 0x8000) {
        u32 i;
        fx32 a;
        fx32 b;
        const s16 *d;
        s32 piv;
        m[0] = m[1] = m[2] = m[3] = m[4] = m[5] = m[6] = m[7] = m[8] = 0;
        i = (info & 0x7fff) * 3;
        d = rot3 + i;
        piv = d[0] & 0xf;
        a = d[1];
        b = d[2];
        m[piv] = (d[0] & 0x10) ? -0x1000 : 0x1000;
        m[data_02072b44[piv].a] = a;
        m[data_02072b44[piv].b] = b;
        if (d[0] & 0x20)
            b = -b;
        m[data_02072b44[piv].c] = b;
        if (d[0] & 0x40)
            a = -a;
        m[data_02072b44[piv].d] = a;
        return FALSE;
    } else {
        u32 i = (info & 0x7fff) * 5;
        const s16 *d = rot5 + i;
        s16 v;
        v = (s16)(d[4] & 7);
        m[4] = d[4] >> 3;
        v = (s16)((v << 3) | (d[0] & 7));
        m[0] = d[0] >> 3;
        v = (s16)((v << 3) | (d[1] & 7));
        m[1] = d[1] >> 3;
        v = (s16)((v << 3) | (d[2] & 7));
        m[2] = d[2] >> 3;
        v = (s16)((v << 3) | (d[3] & 7));
        m[3] = d[3] >> 3;
        m[5] = ((s32)v << 19) >> 19;
        return TRUE;
    }
}
