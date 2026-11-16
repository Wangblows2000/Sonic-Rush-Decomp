#include <nitro/types.h>

typedef void (*UnkCallback_02008098)(void *arg);

typedef struct {
    u32 busy;                       /* 0x00 */
    u32 unk4;
    u32 unk8;
    u32 unkC;
    UnkCallback_02008098 callback;  /* 0x10 */
    void *arg;                      /* 0x14 */
    u32 fifoCond;                   /* 0x18 */
    u32 irqState;                   /* 0x1c */
} UnkStruct_0207d3bc;

extern UnkStruct_0207d3bc data_0207d3bc;

u32 func_02005348(u32 mask);
u32 func_02005520(u32 mask, u32 state);

#define REG_GXSTAT (*(vu32 *)0x04000600)

void func_02008098(void)
{
    func_02005348(0x200000);
    REG_GXSTAT = (data_0207d3bc.fifoCond << 30) | (REG_GXSTAT & ~0xc0000000);
    func_02005520(0x200000, data_0207d3bc.irqState);
    {
        UnkCallback_02008098 cb;
        void *arg;
        data_0207d3bc.busy = 0;
        cb = data_0207d3bc.callback;
        arg = data_0207d3bc.arg;
        if (cb) {
            cb(arg);
        }
    }
}
