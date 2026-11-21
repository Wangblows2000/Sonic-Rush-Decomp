#include <nitro/types.h>

typedef struct { u8 unk0[0x474]; void (*fn)(void); } UnkStruct_0205aff8;
void StageTitleCard_State_StartStage(void);
void StageTitleCard_State_ToStartStage(UnkStruct_0205aff8 *s) { s->fn = StageTitleCard_State_StartStage; }
