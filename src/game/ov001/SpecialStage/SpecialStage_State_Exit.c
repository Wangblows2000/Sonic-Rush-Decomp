/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 SpecialStage_Shutdown();
extern u8 gGameState[];
s32 SceneChange_ToStageFromSpecialStage();

void SpecialStage_State_Exit(void) {
    SpecialStage_Shutdown();
    s32 t0 = (*(u32 *)((u8 *)gGameState + 0x18) & 0x40);
    if (t0 == 0) {
        return;
    }
    SceneChange_ToStageFromSpecialStage(t0);
}
