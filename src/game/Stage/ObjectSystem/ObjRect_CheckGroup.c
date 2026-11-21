#include <nitro/types.h>

typedef struct UnkCol_0206db24 {
    s32 x;
    s32 y;
    s16 z;
    u16 flags;
    s16 l;
    s16 t;
    s16 r;
    s16 b;
    s16 zn;
    s16 zf;
    struct UnkCol_0206db24 *owner;
    u8 pad1c[0xc];
    u8 mask;
} UnkCol_0206db24;

u32 ObjRect_CheckFuncCall(UnkCol_0206db24 *a, UnkCol_0206db24 *b);

void ObjRect_CheckGroup(UnkCol_0206db24 **listA, UnkCol_0206db24 **listB, u8 countA, u8 countB, u8 bit) {
    u8 i;
    u8 j;
    UnkCol_0206db24 *a;
    UnkCol_0206db24 *b;
    s32 ax, ay, az;
    u16 aw, ah, ad;
    s32 bx, by, bz;
    u16 bw, bh, bd;
    u32 r;

    for (i = 0; i < countA; i++) {
        a = listA[i];
        if (a == NULL || (a->flags & 0x1000) || !(a->flags & 4) || !((a->mask >> 4) & (1 << bit)))
            continue;
        if (a->flags & 1)
            ax = a->x - a->r;
        else
            ax = a->x + a->l;
        if (a->flags & 2)
            ay = a->y - a->b;
        else
            ay = a->y + a->t;
        az = a->z + a->zn;
        aw = a->r - a->l;
        ah = a->b - a->t;
        ad = a->zf - a->zn;
        for (j = 0; j < countB; j++) {
            b = listB[j];
            if (listA[i] == NULL)
                break;
            if (b == NULL || ((b->flags | a->flags) & 0x1000) || !(b->flags & 4) || b == a->owner)
                continue;
            if (b->flags & 1)
                bx = b->x - b->r;
            else
                bx = b->x + b->l;
            if (b->flags & 2)
                by = b->y - b->b;
            else
                by = b->y + b->t;
            bz = b->z + b->zn;
            bw = b->r - b->l;
            bh = b->b - b->t;
            bd = b->zf - b->zn;
            if (((ax <= bx && ax + aw >= bx) || (ax >= bx && bx + bw >= ax))
                && ((ay <= by && ay + ah >= by) || (ay >= by && by + bh >= ay))
                && ((az <= bz && az + ad >= bz) || (az >= bz && bz + bd >= az))) {
                r = ObjRect_CheckFuncCall(a, b);
                if (r & 1)
                    listA[i] = NULL;
                if (r & 2)
                    listB[j] = NULL;
                r = ObjRect_CheckFuncCall(b, a);
                if (r & 1)
                    listB[j] = NULL;
                if (r & 2)
                    listA[i] = NULL;
            }
            if (b->flags & 0x400) {
                b->flags |= 0x200;
                b->flags &= ~0x400;
            }
        }
        if (a->flags & 0x400) {
            a->flags |= 0x200;
            a->flags &= ~0x400;
        }
    }
}
