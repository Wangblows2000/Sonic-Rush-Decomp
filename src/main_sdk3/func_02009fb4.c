#include <nitro/types.h>

typedef union {
    u32 raw;
    struct {
        u32 tag : 5;
        u32 err : 1;
        u32 data : 26;
    } e;
} UnkPXIMsg_02009fb4;

typedef void (*UnkPXICb_02009fb4)(u32 tag, u32 data, BOOL err);

extern UnkPXICb_02009fb4 data_0207ef28[];

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);

#define REG_PXI_FIFO_CNT (*(vu16 *)0x04000184)
#define REG_PXI_SEND_FIFO (*(vu32 *)0x04000188)
#define REG_PXI_RECV_FIFO (*(vu32 *)0x04100000)

static inline s32 func_02009fb4_Get(u32 *data)
{
    u32 enabled;
    if (REG_PXI_FIFO_CNT & 0x4000) {
        REG_PXI_FIFO_CNT |= 0xc000;
        return -3;
    }
    enabled = OS_DisableInterrupts();
    if (REG_PXI_FIFO_CNT & 0x100) {
        (void)OS_RestoreInterrupts(enabled);
        return -4;
    }
    *data = REG_PXI_RECV_FIFO;
    (void)OS_RestoreInterrupts(enabled);
    return 0;
}

static inline s32 func_02009fb4_Set(u32 data)
{
    u32 enabled;
    if (REG_PXI_FIFO_CNT & 0x4000) {
        REG_PXI_FIFO_CNT |= 0xc000;
        return -3;
    }
    enabled = OS_DisableInterrupts();
    if (REG_PXI_FIFO_CNT & 0x2) {
        (void)OS_RestoreInterrupts(enabled);
        return -2;
    }
    REG_PXI_SEND_FIFO = data;
    (void)OS_RestoreInterrupts(enabled);
    return 0;
}

void func_02009fb4(void)
{
    UnkPXIMsg_02009fb4 msg;
    s32 ret;
    UnkPXICb_02009fb4 callback;

    while (1) {
        ret = func_02009fb4_Get(&msg.raw);
        if (ret == -4) {
            return;
        }
        if (ret == -3) {
            continue;
        }
        if (msg.e.tag == 0) {
            continue;
        }
        callback = data_0207ef28[msg.e.tag];
        if (callback) {
            callback(msg.e.tag, msg.e.data, msg.e.err);
        } else {
            if (msg.e.err) {
            } else {
                msg.e.err = TRUE;
                (void)func_02009fb4_Set(msg.raw);
            }
        }
    }
}
