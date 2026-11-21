#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[0x9a]; s16 unk9E; } UnkObj_0205c470;
void StageTitleCardPartE0_State_Enter(void);
void StageTitleCardPartE0_State_Delay(void *a, UnkObj_0205c470 *o) { if (o->unk9E++ > 40) { o->unk9E = 0; o->fn = StageTitleCardPartE0_State_Enter; } }
