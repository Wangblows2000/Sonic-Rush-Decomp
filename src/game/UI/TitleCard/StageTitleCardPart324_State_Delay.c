#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0xbc]; s16 unkC0; } UnkObj_0205bb7c;
void StageTitleCardPart324_State_Enter(void);
void StageTitleCardPart324_State_Delay(void *a, UnkObj_0205bb7c *o) { if (o->unkC0++ > 15) { o->unkC0 = 0; o->fn = StageTitleCardPart324_State_Enter; } }
