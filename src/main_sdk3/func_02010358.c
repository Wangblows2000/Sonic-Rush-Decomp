#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    vu32 src;         /* 0x18 */
    vu32 dst;         /* 0x1c */
    vu32 len;         /* 0x20 */
} UnkCardCommon_02010358;

typedef struct {
    u8 pad0[8];
    u32 page;         /* 0x08 */
    u8 padc[0x14];
    u8 buf[0x200];    /* 0x20 */
} UnkCardCache_02010358;

extern UnkCardCommon_02010358 data_0207f2c0;
extern void MI_CpuCopy8(const void *src, void *dst, u32 size);

BOOL func_02010358(UnkCardCache_02010358 *cache)
{
    UnkCardCommon_02010358 *const p = &data_0207f2c0;
    u32 page = p->src & ~(0x200 - 1);
    if (page == cache->page) {
        u32 ofs = p->src - page;
        u32 len = 0x200 - ofs;
        {
            u32 rest = p->len;
            if (len > rest) {
                len = rest;
            }
        }
        MI_CpuCopy8(cache->buf + ofs, (void *)p->dst, len);
        p->src += len;
        p->dst += len;
        p->len -= len;
    }
    return p->len != 0;
}
