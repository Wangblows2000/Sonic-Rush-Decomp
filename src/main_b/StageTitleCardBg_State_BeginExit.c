#include <nitro/types.h>

typedef struct UnkObj_0205c67c { void (*fn)(void *, struct UnkObj_0205c67c *); u8 unk4[8]; u16 unkC; } UnkObj_0205c67c;
void StageTitleCardBg_State_Exit(void *a, UnkObj_0205c67c *o);
void StageTitleCardBg_State_BeginExit(void *a, UnkObj_0205c67c *o) { o->unkC = 0; o->fn = StageTitleCardBg_State_Exit; o->fn(a, o); }
