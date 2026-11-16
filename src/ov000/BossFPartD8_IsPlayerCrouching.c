#include <nitro/types.h>

typedef struct UnkStruct_022f8f00 {
    u8 pad[0x328];
    s16 unk328;
} UnkStruct_022f8f00;

extern UnkStruct_022f8f00 *gPlayers;

BOOL BossFPartD8_IsPlayerCrouching(void) {
    return (u16)(s16)(gPlayers->unk328 - 3) <= 1;
}
