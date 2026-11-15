#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
typedef struct UnkStruct_0200943c { struct UnkStruct_0200943c *next; } UnkStruct_0200943c;
extern UnkStruct_0200943c *data_0207d400;
extern UnkStruct_0200943c *data_0207d3fc;
void func_0200943c(UnkStruct_0200943c *p) {
    u32 e = OS_DisableInterrupts();
    if (data_0207d400 == 0) { data_0207d400 = p; data_0207d3fc = p; }
    else { data_0207d400->next = p; data_0207d400 = p; }
    p->next = 0;
    OS_RestoreInterrupts(e);
}
