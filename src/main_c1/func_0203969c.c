#include <nitro/types.h>

typedef struct {
    u8 pad0[0x28];
    u8 numBanks;
} UnkVramCfg_0203969c;

typedef struct {
    u8 pad0[0x2c];
    u16 unk2c;
    u16 pad2e;
    u16 unk30;
    u16 unk32;
    u16 unk34;
} UnkVramStat_0203969c;

extern UnkVramCfg_0203969c data_0208a754;
extern u16 data_0208a77e[];
extern u16 data_0208a97e[];
extern u16 data_0208ab7e[];
extern u16 data_0208ad7e[];
extern u16 data_0208af7e[];
extern UnkVramStat_0203969c data_0208af54;
extern u16 data_0208af86[];

u32 func_02006abc(u32 a);

u32 func_0203969c(u32 size, BOOL pair)
{
    u16 *tbl;
    u32 k;
    u32 i;
    u32 n;
    u8 bank;
    u32 cnt;
    u16 *sub;
    u32 j;
    u32 base;
    u32 half;
    u32 end;
    u32 m;

    n = (size + 0x1ff) >> 9;
    half = (n + 1) >> 1;
    if (pair) {
        j = 0;
        sub = data_0208a97e;
        cnt = data_0208a754.numBanks;
        for (bank = 0; bank < 2; bank++) {
            if (bank == 0) {
                if (cnt < 2)
                    break;
                tbl = data_0208a77e;
            } else {
                if (cnt < 3)
                    break;
                tbl = data_0208ab7e;
            }
            for (i = 0; i < 0x100; i += tbl[i] & ~0x8000) {
                if (tbl[i] == 0) {
                    for (k = 1; k < n; k++) {
                        if (i + k >= 0x100)
                            goto next_pair;
                        if (tbl[i + k] != 0)
                            break;
                    }
                    if (k == n) {
                        base = (bank ? 0x80 : 0) + (i >> 1);
                        end = base + half;
                        for (; j <= base; j += sub[j] & ~0x8000) {
                            for (m = 1; j + m < end; m++) {
                                if (sub[j + m] != 0) {
                                    j += m;
                                    break;
                                }
                            }
                            if (end == j + m) {
                                tbl[i] = n | 0x8000;
                                data_0208af7e[bank ? 2 : 0] += (u16)n;
                                sub[end - half] = half | 0x8000;
                                data_0208af54.unk2c += (u16)half;
                                data_0208af86[bank] += (u16)half;
                                return (bank ? 0x40000 : 0) + (i << 9);
                            }
                        }
                    }
                }
            }
        next_pair:;
        }
    } else {
        cnt = data_0208a754.numBanks;
        if (cnt >= 4) {
            tbl = data_0208ad7e;
            for (i = 0; i < 0x100; i += tbl[i] & ~0x8000) {
                if (tbl[i] == 0) {
                    for (k = 1; k < n; k++) {
                        if (i + k >= 0x100)
                            goto try_main;
                        if (tbl[i + k] != 0) {
                            i += k;
                            break;
                        }
                    }
                    if (k == n) {
                        tbl[i + k] = n;
                        data_0208af54.unk30 += (u16)n;
                        return (i << 9) + 0x60000;
                    }
                }
            }
        }
    try_main:
        for (bank = 0; bank < 2; bank++) {
            if (bank == 0) {
                if (cnt < 1)
                    break;
                tbl = data_0208a77e;
            } else {
                if (cnt < 3)
                    break;
                tbl = data_0208ab7e;
            }
            for (i = 0; i < 0x100; i += tbl[i] & ~0x8000) {
                if (tbl[i] == 0) {
                    for (k = 1; k < n; k++) {
                        if (i + k >= 0x100)
                            goto next_main;
                        if (tbl[i + k] != 0) {
                            i += k;
                            break;
                        }
                    }
                    if (k == n) {
                        tbl[i] = n;
                        data_0208af7e[bank ? 2 : 0] = n;
                        return (bank ? 0x40000 : 0) + (i << 9);
                    }
                }
            }
        next_main:;
        }
        if (cnt >= 2) {
            tbl = data_0208a97e;
            for (i = 0; i < 0x100; i += tbl[i] & ~0x8000) {
                if (tbl[i] == 0) {
                    for (k = 1; k < n; k++) {
                        if (i + k >= 0x100)
                            goto fail;
                        if (tbl[i + k] != 0) {
                            i += k;
                            break;
                        }
                    }
                    if (k == n) {
                        tbl[i] = n;
                        data_0208af54.unk2c += (u16)n;
                        if (i < 0x80)
                            data_0208af54.unk32 += (u16)n;
                        else
                            data_0208af54.unk34 += (u16)n;
                        return (i << 9) + 0x20000;
                    }
                }
            }
        }
    }
fail:
    return func_02006abc(0);
}
