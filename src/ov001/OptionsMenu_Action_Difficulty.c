#include <nitro/types.h>

typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; void *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void OptionsMenu_InitEnterItemAnims(void *p);
void OptionsMenu_State_Unk_ov001_022cd558(void);

#pragma thumb on
void OptionsMenu_Action_Difficulty(void) {
    OptionsMenu_InitEnterItemAnims(gTaskCurrent->p);
    gTaskCurrent->fn = OptionsMenu_State_Unk_ov001_022cd558;
}
#pragma thumb reset
