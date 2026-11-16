#include <nitro/types.h>

typedef struct { u32 idx; u32 data[0xC0]; } UnkGeBuf_02025268;

extern UnkGeBuf_02025268 *data_02087ca0;
extern volatile u32 data_02087ca4;

void MIi_CpuCopyFast(const void *src, void *dst, u32 size);
void NNS_G3dGeFlushBuffer(void);
void NNS_G3dGeWaitSendDL(void);
void MIi_CpuSend32(const void *src, volatile void *dst, u32 size);

#define REG_GXFIFO (*(vu32 *)0x04000400)

void NNS_G3dGeBufferOP_N(u32 op, const u32 *args, u32 n) {
    UnkGeBuf_02025268 *buf = data_02087ca0;
    if (buf != NULL) {
        if (data_02087ca4 != 0) {
            u32 idx = buf->idx;
            if (idx + 1 + n <= 0xC0) {
                buf->idx = idx + 1;
                data_02087ca0->data[idx] = op;
                if (n != 0) {
                    MIi_CpuCopyFast(args, &data_02087ca0->data[data_02087ca0->idx], n * 4);
                    data_02087ca0->idx += n;
                }
                return;
            }
        }
        if (buf->idx != 0) {
            NNS_G3dGeFlushBuffer();
        } else if (data_02087ca4 != 0) {
            NNS_G3dGeWaitSendDL();
        }
    } else if (data_02087ca4 != 0) {
        NNS_G3dGeWaitSendDL();
    }
    REG_GXFIFO = op;
    MIi_CpuSend32(args, &REG_GXFIFO, n * 4);
}
