#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    vu32 src;         /* 0x18 */
    vu32 dst;         /* 0x1c */
    vu32 len;         /* 0x20 */
    u32 dma;          /* 0x24 */
    void *callback;   /* 0x28 */
    void *arg;        /* 0x2c */
    u8 pad30[4];
    u32 flag;         /* 0x34 */
} UnkCardCommon_0200fd28;

typedef struct {
    BOOL (*read_func)(void *p);
} UnkCardRom_0200fd28;

extern UnkCardCommon_0200fd28 data_0207f2c0;
extern u32 data_0207f8a0;
extern UnkCardRom_0200fd28 data_0207f8c0;

extern void func_0200f358(void);
extern BOOL func_0200ff18(void *p);
extern void func_0201065c(void);

void func_0200fd28(void)
{
    UnkCardCommon_0200fd28 *const p = &data_0207f2c0;
    if (p->flag == 0) {
        p->flag = 1;
        p->src = p->dst = p->len = 0;
        p->dma = (u32)-1;
        p->callback = NULL;
        p->arg = NULL;
        data_0207f8a0 = 0;
        func_0200f358();
        data_0207f8c0.read_func = func_0200ff18;
        func_0201065c();
    }
}
