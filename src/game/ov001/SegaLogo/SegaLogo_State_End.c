#include <nitro/types.h>

typedef struct { s32 timer; } UnkStruct_022cf5d4;
typedef struct { u8 pad[0x10]; UnkStruct_022cf5d4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void Task_DestroyAll(void);
void TitleDemo_Start(void);

#pragma thumb on
void SegaLogo_State_End(void) {
    UnkStruct_022cf5d4 *t = gTaskCurrent->p;
    t->timer++;
    if (t->timer >= 0x1e) {
        t->timer = 0;
        Task_DestroyAll();
        TitleDemo_Start();
    }
}
#pragma thumb reset
