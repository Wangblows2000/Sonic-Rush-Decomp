#include <nitro/types.h>

void Task_Destroy(void *p);
void StaffRoll_State_ReleasePictures(void *p);
void StaffRoll_SetState(void *p, void (*f)(void));

#pragma thumb on
void StaffRoll_State_End(u8 *p) {
    if (*(void **)(p + 8) != NULL) {
        Task_Destroy(*(void **)(p + 8));
        *(void **)(p + 8) = NULL;
    }
    StaffRoll_State_ReleasePictures(p);
    StaffRoll_SetState(p, NULL);
}
#pragma thumb reset
