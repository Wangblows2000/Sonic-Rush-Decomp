#include <nitro/types.h>

typedef struct { u8 pad[0x14]; s32 a; } UnkStruct_022cdbc4;
typedef struct { u8 pad[0x10]; UnkStruct_022cdbc4 *p; } UnkStruct_022b4574;
extern UnkStruct_022b4574 *gTaskCurrent;
BOOL OptionsMenu_UpdateAnims(void *p);
void OptionsMenu_DrawItems(void *p);
void Options_Unk_ov001_022cdfb4(void);

#pragma thumb on
void Options_Unk_ov001_022cdbc4(void) {
    UnkStruct_022cdbc4 *t = gTaskCurrent->p;
    BOOL r = OptionsMenu_UpdateAnims(t);
    OptionsMenu_DrawItems(t);
    t->a++;
    if (r) {
        Options_Unk_ov001_022cdfb4();
    }
}
#pragma thumb reset
