#include <nitro/types.h>

typedef struct { u16 unk0; u16 unk2; } UnkStruct_0203fbd8;
extern void (*sTraceFunc)(void *);
extern u8 data_020789dc;
void WH_SetError(s32 v);
void WH_Reset(void);
BOOL WH_StateInEndParent(void);
void WH_StateOutEndParentMP(UnkStruct_0203fbd8 *s) { if (s->unk2 != 0) { WH_SetError(s->unk2); WH_Reset(); return; } if (WH_StateInEndParent()) return; if (sTraceFunc != NULL) sTraceFunc(&data_020789dc); WH_Reset(); }
