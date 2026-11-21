#include <nitro/types.h>

typedef struct { s32 v; } UnkStruct_022cf2a8;
typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; UnkStruct_022cf2a8 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
BOOL TalkDemo_IsActive(void);
void PostStaffRoll_State_Wait(void);

#pragma thumb on
void PostStaffRoll_State_TalkDemo(void) {
    if (!TalkDemo_IsActive()) {
        gTaskCurrent->p->v = 0;
        gTaskCurrent->fn = PostStaffRoll_State_Wait;
    }
}
#pragma thumb reset
