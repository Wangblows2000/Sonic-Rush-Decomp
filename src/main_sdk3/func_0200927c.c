#include <nitro/types.h>

typedef struct UnkNode_0200927c {
    struct UnkNode_0200927c *next;
} UnkNode_0200927c;

extern UnkNode_0200927c *data_0207d3fc;
extern s32 data_0207d400;
extern s32 data_0207d40c;
extern s32 data_0207d410;
extern s32 data_0207d414;
extern UnkNode_0200927c *data_0207d418[];
extern u8 data_0207d6c0[];

extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern void OS_SpinWait(u32 cycles);
extern UnkNode_0200927c *func_02009520(u32 flag);
extern s32 func_0200a0d8(u32 fifotag, u32 data, BOOL err);
extern void func_02008fe8(void);
extern void DC_FlushRange(const void *p, u32 size);

BOOL func_0200927c(u32 flag)
{
    u32 state = OS_DisableInterrupts();
    if (data_0207d3fc == NULL) {
        OS_RestoreInterrupts(state);
        return TRUE;
    }
    if (data_0207d410 >= 8) {
        if (!(flag & 1)) {
            OS_RestoreInterrupts(state);
            return FALSE;
        }
        do {
            func_02009520(1);
        } while (data_0207d410 >= 8);
    }
    DC_FlushRange(data_0207d6c0, 0x1800);
    if (func_0200a0d8(7, (u32)data_0207d3fc, 0) < 0) {
        if (!(flag & 1)) {
            OS_RestoreInterrupts(state);
            return FALSE;
        }
        while (func_0200a0d8(7, (u32)data_0207d3fc, 0) < 0) {
            OS_RestoreInterrupts(state);
            OS_SpinWait(100);
            state = OS_DisableInterrupts();
        }
    }
    if (flag & 2) {
        func_02008fe8();
    }
    data_0207d418[data_0207d40c] = data_0207d3fc;
    if (++data_0207d40c > 8) {
        data_0207d40c = 0;
    }
    data_0207d3fc = NULL;
    data_0207d400 = 0;
    data_0207d410++;
    data_0207d414++;
    OS_RestoreInterrupts(state);
    return TRUE;
}
