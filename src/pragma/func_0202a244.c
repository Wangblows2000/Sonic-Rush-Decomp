#pragma opt_lifetimes off
#include <nitro/types.h>

void func_0202a244(void *dst, int val, u32 n)
{
    u32 j;
    u32 v;
    u32 i;
    u8 *p;

    p = (u8 *)dst;
    v = (u8)val;
    if (n >= 32) {
        i = (-(u32)p) & 3;
        if (i) {
            n -= i;
            do {
                *p++ = (u8)v;
            } while (--i);
        }
        if (v)
            v |= (v << 24) | (v << 16) | (v << 8);
        i = n >> 5;
        if (i) {
            do {
                ((u32 *)p)[0] = v;
                ((u32 *)p)[1] = v;
                ((u32 *)p)[2] = v;
                ((u32 *)p)[3] = v;
                ((u32 *)p)[4] = v;
                ((u32 *)p)[5] = v;
                ((u32 *)p)[6] = v;
                ((u32 *)p)[7] = v;
                p += 32;
            } while (--i);
        }
        j = (n & 31) >> 2;
        if (j) {
            do {
                *(u32 *)p = v;
                p += 4;
            } while (--j);
        }
        n &= 3;
    }
    if (n) {
        do {
            *p++ = (u8)v;
        } while (--n);
    }
}
