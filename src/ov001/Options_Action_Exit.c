#include <nitro/types.h>

typedef struct { s32 v; } UnkStruct_022cdcf4;
typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; UnkStruct_022cdcf4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void Options_Unk_ov001_022cb834(void *p);
void Options_CreateFadeTask(s32 a, s32 b, s32 c, s32 d);
void Options_State_Exit(void);

#pragma thumb on
void Options_Action_Exit(void) {
    UnkStruct_022cdcf4 *t = gTaskCurrent->p;
    t->v = 0;
    Options_Unk_ov001_022cb834(t);
    Options_CreateFadeTask(0, 0x10, 0x10, 0x1e);
    gTaskCurrent->fn = Options_State_Exit;
}
#pragma thumb reset
