#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203fc88;
extern void (*sTraceFunc)(void *);
extern u8 data_020789f8;
void WH_SetError(s32 v);
void WH_Reset(void);
BOOL WH_StateInEndParentMP(void);
void WH_StateOutEndParentKeyShare(UnkStruct_0203fc88 *s) { if (s->unk2 != 0) { WH_SetError(s->unk2); WH_Reset(); return; } if (WH_StateInEndParentMP()) return; if (sTraceFunc != NULL) sTraceFunc(&data_020789f8); WH_Reset(); }
