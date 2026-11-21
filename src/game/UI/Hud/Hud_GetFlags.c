#include <nitro/types.h>

typedef struct UnkStruct_023376a4 {
    u8 pad[0x10];
    s32 *unk10;
} UnkStruct_023376a4;

extern UnkStruct_023376a4 *gHudTask;

s32 Hud_GetFlags(void) {
    if (gHudTask != NULL)
        return *gHudTask->unk10;
    return 0;
}
