#include <nitro/types.h>

void Task_Destroy(void *p);
void func_ov001_022d2e14(void *p);
void StaffRoll_SetState(void *p, void (*f)(void));

#pragma thumb on
void func_ov001_022d1c9c(u8 *p) {
    if (*(void **)(p + 8) != NULL) {
        Task_Destroy(*(void **)(p + 8));
        *(void **)(p + 8) = NULL;
    }
    func_ov001_022d2e14(p);
    StaffRoll_SetState(p, NULL);
}
#pragma thumb reset
