#include <nitro/types.h>

typedef struct {
    u8 pad0[0xa];
    u16 flags;
} UnkCol_0206ded8;

extern u16 data_022c5664;
extern u16 data_022c5668;
extern u8 data_022c5660;
extern UnkCol_0206ded8 *data_022c597c[4][48];
extern u8 data_022c5678[4];
extern u8 data_022c5670[4];

void func_0206db24(UnkCol_0206ded8 **a, UnkCol_0206ded8 **b, u8 na, u8 nb, u8 j);
void func_0206e130(void);

void func_0206ded8(void) {
    u8 i;
    u8 j;
    UnkCol_0206ded8 *c;

    data_022c5664 = 0;
    data_022c5668 = 0;
    data_022c5660 = 0;
    for (i = 0; i < 4; i++) {
        for (j = 0; j < data_022c5678[i]; j++) {
            c = data_022c597c[i][j];
            if (c != NULL && (c->flags & 0x800))
                c->flags &= ~0x40;
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4; j++) {
            if (data_022c5678[j] != 0 && (data_022c5670[i] & (0x10 << j)))
                func_0206db24(data_022c597c[i], data_022c597c[j], data_022c5678[i], data_022c5678[j], j);
        }
    }
    for (i = 0; i < 4; i++) {
        for (j = 0; j < data_022c5678[i]; j++) {
            c = data_022c597c[i][j];
            if (c != NULL && (c->flags & 0x800) && !(c->flags & 0x40))
                c->flags &= ~0x80;
        }
    }
    func_0206e130();
}
