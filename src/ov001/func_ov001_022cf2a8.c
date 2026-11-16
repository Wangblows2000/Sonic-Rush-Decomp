#include <nitro/types.h>

typedef struct { s32 v; } UnkStruct_022cf2a8;
typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; UnkStruct_022cf2a8 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
BOOL TalkDemo_IsActive(void);
void func_ov001_022cf0e4(void);

#pragma thumb on
void func_ov001_022cf2a8(void) {
    if (!TalkDemo_IsActive()) {
        gTaskCurrent->p->v = 0;
        gTaskCurrent->fn = func_ov001_022cf0e4;
    }
}
#pragma thumb reset
