#include <nitro/types.h>

typedef struct { u32 timer; } UnkStruct_022cf0e4;
typedef struct { u8 pad[0x10]; UnkStruct_022cf0e4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void Task_Destroy(void *p);
void PostStaffRoll_ShowUnlockNotices(s32 a);

#pragma thumb on
void PostStaffRoll_State_Wait(void) {
    UnkStruct_022cf0e4 *t = gTaskCurrent->p;
    t->timer++;
    if (t->timer >= 0x5a) {
        Task_Destroy(gTaskCurrent);
        PostStaffRoll_ShowUnlockNotices(0);
    }
}
#pragma thumb reset
