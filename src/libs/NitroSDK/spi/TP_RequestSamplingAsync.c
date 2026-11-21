#include <nitro/types.h>

typedef void (*UnkTPCallback_0200d650)(u32 command, u32 result, u16 index);

typedef struct {
    UnkTPCallback_0200d650 callback;   /* 0x00 */
    u8 pad4[0x30];
    vu16 err;     /* 0x34 */
    vu16 command; /* 0x36 */
} UnkTPWork_0200d650;

extern UnkTPWork_0200d650 data_0207f02c;
extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern s32 PXI_SendWordByFifo(u32 fifotag, u32 data, BOOL err);

static inline BOOL SendData(u32 data)
{
    return (PXI_SendWordByFifo(6, data, 0) >= 0) ? TRUE : FALSE;
}

void TP_RequestSamplingAsync(void)
{
    u32 state = OS_DisableInterrupts();
    if (SendData(0x03000000) == FALSE) {
        OS_RestoreInterrupts(state);
        data_0207f02c.err |= 1;
        if (data_0207f02c.callback) {
            data_0207f02c.callback(0, 4, 0);
        }
        return;
    }
    data_0207f02c.command |= 1;
    data_0207f02c.err &= ~1;
    OS_RestoreInterrupts(state);
}
