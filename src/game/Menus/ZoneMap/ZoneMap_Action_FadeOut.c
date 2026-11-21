#include <nitro/types.h>

typedef struct { u8 pad[0x74]; s32 v; } UnkStruct_022e5b6c;
typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; UnkStruct_022e5b6c *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void ZoneMap_CreateFadeTask(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void ZoneMap_State_Unk_ov001_022e7428(void);

#pragma thumb on
void ZoneMap_Action_FadeOut(void) {
    gTaskCurrent->p->v = 0;
    ZoneMap_CreateFadeTask(0, 0x10, 0x10, 0, 1, 0);
    gTaskCurrent->fn = ZoneMap_State_Unk_ov001_022e7428;
}
#pragma thumb reset
