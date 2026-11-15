#include <nitro/types.h>
#include <main_sdk2/stdarg.h>

typedef struct {
    u8 pad0[0xc];
    u32 *cmdBuf;   /* 0xc */
    u8 pad10[4];
    u16 dma;       /* 0x14 */
} UnkWmSys_02010b80;

extern UnkWmSys_02010b80 *data_0207faec;
extern u8 data_0207faf0[];

BOOL func_020063f4(void *queue, void *msg, s32 flags);
BOOL func_02006340(void *queue, void *msg, s32 flags);
BOOL func_020064a0(void *queue, void *msg, s32 flags);
void DC_InvalidateRange(void *p, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_02007da4(u32 dma, const void *src, void *dst, u32 size);
s32 func_0200a0d8(u32 fifo, void *data, BOOL err);

s32 func_02010b80(u16 cmd, u16 paramNum, ...)
{
    u32 *buf;
    va_list ap;
    s32 i;
    s32 r;

    if (!func_020063f4(data_0207faf0, &buf, 0)) {
        return 8;
    }
    DC_InvalidateRange(buf, 2);
    if (!(*(u16 *)buf & 0x8000)) {
        func_02006340(data_0207faf0, buf, 1);
        return 8;
    }
    if (paramNum == 0) {
        DC_StoreRange(data_0207faec->cmdBuf, 0x100);
        DC_InvalidateRange(buf, 0x100);
        func_02007da4(data_0207faec->dma, data_0207faec->cmdBuf, buf, 0x100);
    }
    *(u16 *)buf = cmd;
    va_start(ap, paramNum);
    for (i = 0; i < paramNum; i++) {
        buf[i + 1] = va_arg(ap, u32);
    }
    va_end(ap);
    DC_StoreRange(buf, 0x100);
    r = func_0200a0d8(10, buf, FALSE);
    func_020064a0(data_0207faf0, buf, 1);
    return r < 0 ? 8 : 2;
}
