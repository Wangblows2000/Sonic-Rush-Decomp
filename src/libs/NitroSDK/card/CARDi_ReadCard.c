#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    u32 src; /* 0x18 */
    u32 dst; /* 0x1c */
    u32 len; /* 0x20 */
} UnkCardCommon_0200ff18;

typedef struct {
    u32 unk0;
    u32 ctrl;        /* 0x04 */
    void *cache_page; /* 0x08 */
    u8 padC[0x14];
    u8 cache_buf[512]; /* 0x20 */
} UnkCardRomStat_0200ff18;

extern UnkCardCommon_0200ff18 data_0207f2c0;

void CARDi_SetRomOp(u32 cmd1, u32 cmd2);
BOOL CARDi_ReadFromCache(UnkCardRomStat_0200ff18 *p);

static inline void CARDi_SetRomOpReadPage1_0200ff18(u32 src)
{
    CARDi_SetRomOp((u32)(0xb7000000 | (src >> 8)), (u32)(src << 24));
}

static inline BOOL CARDi_OnReadPageDirect_0200ff18(UnkCardRomStat_0200ff18 *arg)
{
    UnkCardCommon_0200ff18 *p = &data_0207f2c0;
    (void)arg;
    p->src += 0x200;
    p->dst += 0x200;
    p->len -= 0x200;
    return (p->len > 0);
}

void CARDi_ReadCard(UnkCardRomStat_0200ff18 *p)
{
    UnkCardCommon_0200ff18 *const c = &data_0207f2c0;
    for (;;) {
        const u32 len = 0x200;
        u32 src = c->src & ~(0x200 - 1);
        u32 dst;

        if ((src != c->src) || ((c->dst & 3) != 0) || (c->len < len)) {
            dst = (u32)p->cache_buf;
            p->cache_page = (void *)src;
        } else {
            dst = c->dst;
        }

        CARDi_SetRomOpReadPage1_0200ff18(src);
        {
            u32 pos = 0;
            *(vu32 *)0x040001a4 = p->ctrl;
            for (;;) {
                const u32 ctrl = *(vu32 *)0x040001a4;
                if ((ctrl & 0x800000) != 0) {
                    u32 data = *(vu32 *)0x04100010;
                    if (pos < len)
                        ((u32 *)dst)[pos++] = data;
                }
                if (!(ctrl & 0x80000000))
                    break;
            }
        }

        if (dst == c->dst) {
            if (!CARDi_OnReadPageDirect_0200ff18(p))
                break;
        } else {
            if (!CARDi_ReadFromCache(p))
                break;
        }
    }
}
