#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
typedef struct UnkStruct_020090d8 { struct UnkStruct_020090d8 *next; } UnkStruct_020090d8;
extern UnkStruct_020090d8 *data_0207d3fc;
s32 SND_CountReservedCommand(void) { u32 e = OS_DisableInterrupts(); s32 n = 0; UnkStruct_020090d8 *p; for (p = data_0207d3fc; p != 0; p = p->next) n++; OS_RestoreInterrupts(e); return n; }
