#include <nitro/types.h>

typedef struct {
    u8 pad[0x29];
    u8 level;
} UnkStruct_0208a754;

extern UnkStruct_0208a754 data_0208a754;
extern u16 data_0208ef8a[2][0x200];
extern u8 data_0207320c[2];
extern u16 data_0208af8a[4][0x800];
extern u8 data_02073210[4];
u32 func_02006abc(u32 a);

u32 VramPltt_Alloc(u32 size, BOOL flag) {
    u16 *tbl;
    u32 j;
    u32 i;
    u32 n;
    u8 b;
    u8 lvl;
    if (flag == 0) {
        lvl = data_0208a754.level;
        n = (size + 15) >> 4;
        tbl = data_0208ef8a[0];
        for (b = 0; b < 2; b++, tbl += 0x200) {
            if (data_0207320c[b] > lvl)
                break;
            for (i = 0; i < 0x200; i += tbl[i]) {
                if (tbl[i] == 0) {
                    for (j = 1; j < n; j++) {
                        if (i + j >= 0x200)
                            goto next1;
                        if (tbl[i + j] != 0) {
                            i += j;
                            break;
                        }
                    }
                    if (j == n) {
                        tbl[i] = n;
                        return ((b + 4) << 14) + (i << 5);
                    }
                }
            }
        next1:;
        }
    }
    {
        u8 lvl2; u16 *tbl2; u32 n2;
        lvl2 = data_0208a754.level;
        n2 = (size + 3) >> 2;
        tbl2 = data_0208af8a[0];
        for (b = 0; b < 4 && b < 4; tbl2 += 0x800, b++) {
            if (data_02073210[b] > lvl2)
                break;
            for (i = 0; i < 0x800; i += tbl2[i]) {
                if (tbl2[i] == 0) {
                    for (j = 1; j < n2; j++) {
                        if (i + j >= 0x800)
                            goto next2;
                        if (tbl2[i + j] != 0) {
                            i += j;
                            break;
                        }
                    }
                    if (j == n2) {
                        tbl2[i] = n2;
                        return (b << 14) + (i << 3);
                    }
                }
            }
        next2:;
        }
    }
    return func_02006abc(0);
}
