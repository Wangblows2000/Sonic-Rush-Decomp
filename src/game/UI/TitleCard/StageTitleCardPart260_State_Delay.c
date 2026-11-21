#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0xbc]; s16 unkC0; } UnkObj_0205bf5c;
void StageTitleCardPart260_State_Enter(void);
void StageTitleCardPart260_State_Delay(void *a, UnkObj_0205bf5c *o) { if (o->unkC0++ > 0) { o->unkC0 = 0; o->fn = StageTitleCardPart260_State_Enter; } }
