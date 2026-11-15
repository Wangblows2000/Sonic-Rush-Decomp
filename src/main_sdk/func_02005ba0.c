#include <nitro/types.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void func_02005a94(void);
typedef struct { u8 pad[0x64]; u32 state; } UnkStruct_02005ba0;
void func_02005ba0(UnkStruct_02005ba0 *p) { u32 e = OS_DisableInterrupts(); p->state = 1; func_02005a94(); OS_RestoreInterrupts(e); }
