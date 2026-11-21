#include <nitro/types.h>

typedef struct { u8 pad[0x28]; s32 v; } UnkStruct_022cddc4;
typedef struct { u8 pad[8]; void (*fn)(void); u8 pad2[4]; UnkStruct_022cddc4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
void OptionsMenu_InitEnterItemAnims(void *p);
void OptionsMenu_State_EraseData(void);

#pragma thumb on
void OptionsMenu_Action_EraseData(void) {
    UnkStruct_022cddc4 *t = gTaskCurrent->p;
    OptionsMenu_InitEnterItemAnims(t);
    t->v = -1;
    gTaskCurrent->fn = OptionsMenu_State_EraseData;
}
#pragma thumb reset
