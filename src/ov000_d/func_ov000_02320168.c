#include <nitro/types.h>

typedef struct {
    u8 pad0[8];
    u32 unk8;
    u32 unkC;
    u8 pad10[0x54 - 0x10];
    u32 unk54;
} UnkStruct_02320168;

void func_ov000_02320344(UnkStruct_02320168 *w, u16 i);

void func_ov000_02320168(UnkStruct_02320168 *w) {
    s32 i;
    u32 *p;
    u32 t;

    if (!(w->unk54 & 0x1000)) {
        w->unk54 |= 0x1000;
        for (i = 0; i < 0xc; i++) {
            func_ov000_02320344(w, i);
        }
        for (i = 0xd; i < 0x19; i++) {
            func_ov000_02320344(w, i);
        }
    }
    p = &w->unk8;
    w->unk8 &= 0xffff;
    t = w->unkC;
    if (t & 0x100) {
        if (!(t & 7)) {
            *p <<= 1;
            if (!(w->unk8 & 0x8000) || (w->unkC & 0xff) == 0xf8) {
                w->unk8 |= 1;
            }
        }
        if (!(w->unkC & 0xff)) {
            w->unk8 = 0xffff;
        }
    } else if (!(t & 0x80)) {
        if (!(t & 7)) {
            *p >>= 1;
        }
        if (!(w->unkC & 0x7f)) {
            w->unk8 = 0xfff;
        }
    } else if (!(t & 0x40)) {
        if (!(t & 3)) {
            *p <<= 1;
            if (!(w->unk8 & 2)) {
                *p |= 1;
            }
        }
    } else if (!(t & 3)) {
        *p <<= 1;
        *p |= 1;
    }
    w->unk8 &= 0xffff;
    w->unk8 |= ~w->unk8 << 16;
    w->unkC++;
}
