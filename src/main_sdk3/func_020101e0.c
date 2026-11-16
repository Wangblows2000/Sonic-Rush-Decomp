#include <nitro/types.h>

typedef struct {
    u8 pad0[0x18];
    vu32 src;         /* 0x18 */
    vu32 dst;         /* 0x1c */
    vu32 len;         /* 0x20 */
    u32 dma;          /* 0x24 */
} UnkCardCommon_020101e0;

typedef struct {
    void *read_func;
    u32 ctrl;
} UnkCardRom_020101e0;

extern UnkCardCommon_020101e0 data_0207f2c0;
extern UnkCardRom_020101e0 data_0207f8c0;

extern void func_02008860(u32 dmaNo, u32 src, u32 dest, u32 size);
extern void CARDi_SetRomOp(u32 cmd1, u32 cmd2);

void func_020101e0(void)
{
    UnkCardCommon_020101e0 *const p = &data_0207f2c0;
    func_02008860(p->dma, 0x04100010, p->dst, 0x200);
    {
        u32 src = p->src;
        CARDi_SetRomOp((src >> 8) | 0xb7000000, src << 24);
    }
    *(vu32 *)0x040001a4 = data_0207f8c0.ctrl;
}
