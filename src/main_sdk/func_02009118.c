#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
typedef struct UnkStruct_02009118 { struct UnkStruct_02009118 *next; } UnkStruct_02009118;
extern UnkStruct_02009118 *data_0207d3f4;
s32 func_02009118(void) { u32 e = OS_DisableInterrupts(); s32 n = 0; UnkStruct_02009118 *p; for (p = data_0207d3f4; p != 0; p = p->next) n++; OS_RestoreInterrupts(e); return n; }
