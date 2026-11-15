#include <nitro/types.h>

typedef struct { u8 unk0[0x850]; u32 unk850; } UnkStruct_022c0be0;
extern UnkStruct_022c0be0 data_022c0be0;
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 state);
void func_0203c254(u32 v) { u32 e = OS_DisableInterrupts(); data_022c0be0.unk850 = v & ~1; OS_RestoreInterrupts(e); }
