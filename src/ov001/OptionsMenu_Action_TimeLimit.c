#include <nitro/types.h>

typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; void *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void OptionsMenu_Unk_ov001_022cb770(void *p);
void OptionsMenu_State_TimeLimit(void);

#pragma thumb on
void OptionsMenu_Action_TimeLimit(void) {
    OptionsMenu_Unk_ov001_022cb770(gTaskCurrent->p);
    gTaskCurrent->fn = OptionsMenu_State_TimeLimit;
}
#pragma thumb reset
