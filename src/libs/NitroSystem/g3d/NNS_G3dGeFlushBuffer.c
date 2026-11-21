#include <nitro/types.h>

extern u32 data_02087ca4;
extern u32 *data_02087ca0;
void NNS_G3dGeWaitSendDL(void);
void MIi_CpuSend32(const void *src, volatile void *dest, u32 size);

void NNS_G3dGeFlushBuffer(void) {
    u32 *buf;
    if (data_02087ca4 != 0) NNS_G3dGeWaitSendDL();
    buf = data_02087ca0;
    if (buf == NULL) return;
    if (buf[0] == 0) return;
    MIi_CpuSend32(buf + 1, (volatile void *)0x04000400, buf[0] << 2);
    *data_02087ca0 = 0;
}
