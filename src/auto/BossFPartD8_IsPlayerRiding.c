/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
extern u8 gBossF[];

s32 BossFPartD8_IsPlayerRiding(void) {
    s32 t0 = *(u32 *)gPlayers;
    if ((*(u32 *)((u8 *)*(u32 *)gPlayers + 0x58) & 1) != 0) {
        s32 t1 = *(u32 *)((u8 *)*(u32 *)gBossF + 0x218);
        if (*(u32 *)((u8 *)t0 + 0x80) == t1) {
            return 1;
        }
    }
    return 0;
}
