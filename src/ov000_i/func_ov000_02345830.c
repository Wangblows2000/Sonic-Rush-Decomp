#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u32 unk4;
} UnkCfg_02345830;

extern UnkCfg_02345830 data_022c4590;
void func_ov000_0232b898(u8 *w, u32 a);

void func_ov000_02345830(u8 *w, u32 b) {
    u32 a;

    switch (w[0x3cf]) {
    case 0:
        a = 0;
        break;
    case 1:
        a = 100;
        break;
    case 2:
        a = 200;
        break;
    default:
        a = 300;
        break;
    }
    a >>= w[0x3d0];
    b >>= w[0x3d0];
    func_ov000_0232b898(w, a);
    data_022c4590.unk4 += a;
    data_022c4590.unk4 += b;
    func_ov000_0232b898(w, b);
    w[0x3cf]++;
}
