#include <nitro/types.h>

typedef struct UnkNode_02009520 {
    struct UnkNode_02009520 *next;
} UnkNode_02009520;

extern u32 data_0207d3f8;
extern s32 data_0207d408;
extern UnkNode_02009520 *data_0207d418[];
extern UnkNode_02009520 *data_0207d404;
extern UnkNode_02009520 *data_0207d3f4;
extern s32 data_0207d410;

extern u32 OS_DisableInterrupts(void);
extern u32 OS_RestoreInterrupts(u32 state);
extern void OS_SpinWait(u32 cycles);
extern u32 func_020098cc(void);

UnkNode_02009520 *func_02009520(u32 flag)
{
    UnkNode_02009520 *last;
    u32 state;
    UnkNode_02009520 *p;
    state = OS_DisableInterrupts();
    if (flag & 1) {
        while (func_020098cc() == data_0207d3f8) {
            OS_RestoreInterrupts(state);
            OS_SpinWait(100);
            state = OS_DisableInterrupts();
        }
    } else {
        if (func_020098cc() == data_0207d3f8) {
            OS_RestoreInterrupts(state);
            return NULL;
        }
    }
    p = data_0207d418[data_0207d408];
    if (++data_0207d408 > 8) {
        data_0207d408 = 0;
    }
    for (last = p; last->next != NULL; last = last->next) {
    }
    if (data_0207d404 != NULL) {
        data_0207d404->next = p;
    } else {
        data_0207d3f4 = p;
    }
    data_0207d404 = last;
    data_0207d410--;
    data_0207d3f8++;
    OS_RestoreInterrupts(state);
    return p;
}
