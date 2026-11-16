#include <nitro/types.h>

typedef struct UnkCol_0206d900 UnkCol_0206d900;
struct UnkCol_0206d900 {
    u8 pad0[0xa];
    u16 flags;
    u8 padc[0x10];
    void (*cb)(UnkCol_0206d900 *a, UnkCol_0206d900 *b);
    void *unk20;
    void *unk24;
};

extern u8 data_022c5660;
extern u16 data_022c5664;
extern u16 data_022c5668;

BOOL func_0206daf4(void *a, void *b);

u16 func_0206d900(UnkCol_0206d900 *a, UnkCol_0206d900 *b) {
    u16 ret = 0;
    u16 f;

    data_022c5664 = a->flags;
    data_022c5668 = b->flags;
    if (a == b)
        return 0;
    if ((a->flags & 0x200) && (b->flags & 0x100))
        return 0;
    if (func_0206daf4(a->unk20, b->unk24)) {
        if (!(b->flags & 0x840) && !(a->flags & 0x840))
            a->flags |= 0x400;
        if (!(b->flags & 0x880) && !(a->flags & 0x880))
            b->flags |= 0x100;
        f = a->flags;
        if ((!(f & 0x800) || !(f & 0x80)) && (f & 8)) {
            if (f & 0x800)
                a->flags |= 0x80;
            if (a->cb != NULL)
                a->cb(a, b);
        }
        if (data_022c5660) {
            data_022c5660 = 0;
            return 0;
        }
        data_022c5664 ^= data_022c5668;
        data_022c5668 ^= data_022c5664;
        data_022c5664 ^= data_022c5668;
        if (a->flags & 0x800)
            a->flags |= 0x40;
        f = b->flags;
        if ((!(f & 0x800) || !(f & 0x80)) && !(f & 8)) {
            if (f & 0x800)
                b->flags |= 0x80;
            if (b->cb != NULL)
                b->cb(b, a);
        }
        if (data_022c5660) {
            data_022c5660 = 0;
            return 0;
        }
        if (b->flags & 0x800)
            b->flags |= 0x40;
        if (!(a->flags & 0x20))
            ret |= 1;
        if (!(b->flags & 0x20))
            ret |= 2;
    }
    return ret;
}
