#include <nitro/types.h>

typedef struct {
    u8 pad0[0x94];
    u16 shift;
    u8 pad96[2];
    u16 tbl[0x400];
    u16 base;
    u16 total;
} UnkVram_02039c18;

extern UnkVram_02039c18 *data_02072c78[];
extern u32 data_02072c70[];

u32 func_02006abc(s32 i);

u32 func_02039c18(s32 screen, u32 n) {
    UnkVram_02039c18 *m;
    u32 total;
    u32 i;
    u32 j;

    i = 0;
    m = data_02072c78[screen];
    total = m->total;

    while (i < total) {
        if (m->tbl[i] == 0) {
            for (j = 1; j < n; j++) {
                if (i + j >= total)
                    return func_02006abc(0);
                if (m->tbl[i + j] != 0) {
                    i = i + j;
                    break;
                }
            }
            if (j == n) {
                m->tbl[i] = n;
                return data_02072c70[screen] + (((i + m->base) << m->shift) << 5);
            }
        }
        i += m->tbl[i];
    }
    return func_02006abc(0);
}
