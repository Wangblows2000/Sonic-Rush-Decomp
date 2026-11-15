#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
typedef struct UnkStruct_02008f90 { struct UnkStruct_02008f90 *next; } UnkStruct_02008f90;
extern UnkStruct_02008f90 *data_0207d3f4;
extern UnkStruct_02008f90 *data_0207d404;
UnkStruct_02008f90 *func_02008f90(void) {
    u32 e = OS_DisableInterrupts();
    UnkStruct_02008f90 *p = data_0207d3f4;
    UnkStruct_02008f90 *next;
    if (p == 0) { OS_RestoreInterrupts(e); return 0; }
    next = p->next;
    data_0207d3f4 = next;
    if (next == 0) data_0207d404 = 0;
    OS_RestoreInterrupts(e);
    return p;
}
