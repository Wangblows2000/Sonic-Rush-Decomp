#include <nitro/types.h>

typedef struct {
    u32 wm7buf;      /* 0x00 */
    u32 status;      /* 0x04 */
    u32 pad08;
    u32 fifo9to7;    /* 0x0c */
    u32 fifo7to9;    /* 0x10 */
    u16 dmaNo;       /* 0x14 */
    u8 pad16[0x15c - 0x16];
    u32 field_15c;   /* 0x15c */
} UnkWm9Buf_02010d60;

typedef struct {
    u16 command;
    u8 pad[0x100 - 2];
} UnkWmArm7Cmd_02010d60;

extern u16 data_0207fae8;
extern UnkWm9Buf_02010d60 *data_0207faec;
extern u8 data_0207faf0[];
extern u8 data_0207fb04[];
extern UnkWmArm7Cmd_02010d60 data_0207fb80[10];

extern void PXI_Init(void);
extern BOOL PXI_IsCallbackReady(u32 fifotag, u32 proc);
extern void DC_InvalidateRange(void *p, u32 size);
extern void DC_StoreRange(const void *p, u32 size);
extern void MI_DmaFill32(u32 dmaNo, void *dest, u32 data, u32 size);
extern void func_02010694(void);
extern void func_020114f0(u16 a, u32 b, u32 c);
extern void OS_InitMessageQueue(void *queue, void *msgArray, s32 count);
extern BOOL OS_SendMessage(void *queue, void *msg, s32 flags);
extern void PXI_SetFifoRecvCallback(u32 fifotag, void (*callback)(u32, u32, BOOL));
extern void func_020106b0(u32 tag, u32 data, BOOL err);

s32 func_02010d60(void *wmSysBuf, u16 dmaNo, u32 bufSize)
{
    s32 i;
    UnkWmArm7Cmd_02010d60 *cmd;
    s32 j;
    if (data_0207fae8 != 0) {
        return 3;
    }
    data_0207fae8 = 1;
    if (wmSysBuf == NULL) {
        data_0207fae8 = 0;
        return 6;
    }
    if (dmaNo > 3) {
        data_0207fae8 = 0;
        return 6;
    }
    if ((u32)wmSysBuf & 0x1f) {
        data_0207fae8 = 0;
        return 6;
    }
    PXI_Init();
    if (!PXI_IsCallbackReady(10, 1)) {
        data_0207fae8 = 0;
        return 4;
    }
    DC_InvalidateRange(wmSysBuf, bufSize);
    MI_DmaFill32(dmaNo, wmSysBuf, 0, bufSize);
    data_0207faec = (UnkWm9Buf_02010d60 *)wmSysBuf;
    data_0207faec->wm7buf = (u32)wmSysBuf + 0xa00;
    data_0207faec->status = data_0207faec->wm7buf + 0x300;
    data_0207faec->fifo9to7 = data_0207faec->status + 0x800;
    data_0207faec->fifo7to9 = data_0207faec->fifo9to7 + 0x100;
    func_02010694();
    data_0207faec->dmaNo = dmaNo;
    data_0207faec->field_15c = 0;
    for (i = 0; i < 16; i++) {
        func_020114f0((u16)i, 0, 0);
    }
    OS_InitMessageQueue(data_0207faf0, data_0207fb04, 10);
    for (j = 0; j < 10; j++) {
        cmd = &data_0207fb80[j];
        cmd->command = 0x8000;
        DC_StoreRange(cmd, 2);
        OS_SendMessage(data_0207faf0, cmd, 1);
    }
    PXI_SetFifoRecvCallback(10, func_020106b0);
    return 0;
}
