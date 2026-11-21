#include <nitro/types.h>

typedef struct { u8 pad[8]; void (*fn)(void); } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void StaffRoll_DestroyCreditTask(s32 i);
void StaffRoll_State_Finish(void);

#pragma thumb on
void StaffRoll_State_ReleasePictures(void) {
    s32 i;
    for (i = 0; i < 4; i++) {
        StaffRoll_DestroyCreditTask(i);
    }
    gTaskCurrent->fn = StaffRoll_State_Finish;
}
#pragma thumb reset
