#include <nitro/types.h>

typedef struct { u8 unk0[0x488]; u32 flags; } UnkStruct_0205cf28;
void StageTitleCardPlayer_State_BeginCameraMove(void *a, void *s);
void StageTitleCardPlayer_State_Wait(UnkStruct_0205cf28 *s, void (**p)(void *, void *)) { if (s->flags & 0x100000) *p = StageTitleCardPlayer_State_BeginCameraMove; }
