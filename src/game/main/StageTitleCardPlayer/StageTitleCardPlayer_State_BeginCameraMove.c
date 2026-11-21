#include <nitro/types.h>

typedef struct { void (*fn)(void); u8 unk4[8]; u16 unkC; } UnkStruct_0205cf10;
void StageTitleCardPlayer_State_CameraMove(void);
void StageTitleCardPlayer_State_BeginCameraMove(void *a, UnkStruct_0205cf10 *s) { s->unkC = 0; s->fn = StageTitleCardPlayer_State_CameraMove; }
