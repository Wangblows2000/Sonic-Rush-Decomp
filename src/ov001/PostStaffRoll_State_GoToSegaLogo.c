#include <nitro/types.h>

typedef struct { s32 timer; } UnkStruct_022cf0b4;
typedef struct { u8 pad[0x10]; UnkStruct_022cf0b4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void SysSound_Release(void);
void Task_Destroy(void *p);
void SceneChange_ToSegaLogo(void);

#pragma thumb on
void PostStaffRoll_State_GoToSegaLogo(void) {
    UnkStruct_022cf0b4 *t = gTaskCurrent->p;
    t->timer++;
    if (t->timer >= 0xb4) {
        SysSound_Release();
        Task_Destroy(gTaskCurrent);
        SceneChange_ToSegaLogo();
    }
}
#pragma thumb reset
