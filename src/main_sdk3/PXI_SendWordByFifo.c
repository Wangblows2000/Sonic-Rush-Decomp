#include <nitro/types.h>

typedef union {
    u32 raw;
    struct {
        u32 tag : 5;
        u32 err : 1;
        u32 data : 26;
    } e;
} UnkPXIFifoMessage;

extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);

#define REG_PXI_FIFO_CNT (*(vu16 *)0x04000184)
#define REG_PXI_SEND_FIFO (*(vu32 *)0x04000188)

s32 PXI_SendWordByFifo(u32 fifotag, u32 data, BOOL err)
{
    UnkPXIFifoMessage fifomsg;
    u32 state;

    fifomsg.e.tag = fifotag;
    fifomsg.e.err = err;
    fifomsg.e.data = data;

    if (REG_PXI_FIFO_CNT & 0x4000) {
        REG_PXI_FIFO_CNT |= 0xc000;
        return -1;
    }
    state = OS_DisableInterrupts();
    if (REG_PXI_FIFO_CNT & 2) {
        OS_RestoreInterrupts(state);
        return -2;
    }
    REG_PXI_SEND_FIFO = fifomsg.raw;
    OS_RestoreInterrupts(state);
    return 0;
}
