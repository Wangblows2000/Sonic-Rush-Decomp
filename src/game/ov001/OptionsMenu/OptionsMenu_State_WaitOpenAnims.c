#include <nitro/types.h>

typedef struct { u8 pad[0x14]; s32 a; } UnkStruct_022ccddc;
typedef struct { u8 pad[0x10]; UnkStruct_022ccddc *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
BOOL OptionsMenu_UpdateAnims(void *p);
void OptionsMenu_DrawItems(void *p);
void Options_Action_Select(void);

#pragma thumb on
void OptionsMenu_State_WaitOpenAnims(void) {
    UnkStruct_022ccddc *t = gTaskCurrent->p;
    BOOL r = OptionsMenu_UpdateAnims(t);
    OptionsMenu_DrawItems(t);
    t->a++;
    if (r) {
        Options_Action_Select();
    }
}
#pragma thumb reset
