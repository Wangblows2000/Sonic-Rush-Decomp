#include <nitro/types.h>

typedef void (*UnkPXIFifoCallback)(u32 tag, u32 data, BOOL err);

typedef struct {
    u8 pad0[0x388];
    u32 handlers[2];
} UnkSysWork_0200a1b4;

extern UnkPXIFifoCallback data_0207ef28[32];
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);

void PXI_SetFifoRecvCallback(u32 fifotag, UnkPXIFifoCallback callback)
{
    u32 state = OS_DisableInterrupts();
    data_0207ef28[fifotag] = callback;
    if (callback) {
        UnkSysWork_0200a1b4 *w = (UnkSysWork_0200a1b4 *)0x027ffc00;
        w->handlers[0] |= (1 << fifotag);
    } else {
        UnkSysWork_0200a1b4 *w = (UnkSysWork_0200a1b4 *)0x027ffc00;
        w->handlers[0] &= ~(1 << fifotag);
    }
    OS_RestoreInterrupts(state);
}
