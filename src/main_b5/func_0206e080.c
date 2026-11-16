#include <nitro/types.h>

typedef struct {
    u8 pad0[0xa];
    u16 flags;
    u8 padc[0x1c];
    u8 layers;
} UnkSpr_0206e080;

extern u8 data_022c5674[];
extern UnkSpr_0206e080 *data_022c567c[][0x30];
extern u8 data_022c566c[];

void func_0206e080(UnkSpr_0206e080 *s) {
    u8 i;

    if (!(s->flags & 4))
        return;
    for (i = 0; i < 4; i++) {
        if (s->layers & (1 << i)) {
            if (data_022c5674[i] >= 0x30)
                return;
            data_022c567c[i][data_022c5674[i]] = s;
            data_022c566c[i] |= s->layers;
            data_022c5674[i]++;
            return;
        }
    }
}
