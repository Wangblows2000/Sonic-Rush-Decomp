#include <nitro/types.h>

extern u32 data_02077964;
extern volatile u32 data_02087ca4;
extern u32 data_02087ca8;

void NNS_G3dGeBufferOP_N(u32 op, const u32 *args, u32 n);
void NNS_G3dGeFlushBuffer(void);
void simpleUnlock_(void *arg);
void MI_SendGXCommandAsyncFast(u32 dma, const void *src, u32 size, void (*cb)(void *), void *arg);
void MI_SendGXCommandAsync(u32 dma, const void *src, u32 size, void (*cb)(void *), void *arg);

void NNS_G3dGeSendDL(const u32 *data, u32 size) {
    if (size < 0x100 || data_02077964 == (u32)-1) {
        NNS_G3dGeBufferOP_N(data[0], data + 1, (size >> 2) - 1);
        return;
    }
    NNS_G3dGeFlushBuffer();
    data_02087ca4 = 1;
    if (data_02087ca8 != 0) {
        MI_SendGXCommandAsyncFast(data_02077964, data, size, simpleUnlock_, (void *)&data_02087ca4);
    } else {
        MI_SendGXCommandAsync(data_02077964, data, size, simpleUnlock_, (void *)&data_02087ca4);
    }
}
