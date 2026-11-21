/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gBossHelpTask[];
s32 Task_Destroy();

void BossHelp_Destroy(void) {
    s32 t0 = *(u32 *)gBossHelpTask;
    if (t0 == 0) {
        return;
    }
    Task_Destroy(t0);
    *(u32 *)gBossHelpTask = 0;
}
