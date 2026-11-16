/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gPlayers[];
extern u8 data_ov000_02369b84[];

s32 func_ov000_022f8f38(void) {
    s32 t0 = *(u32 *)gPlayers;
    if ((*(u32 *)((u8 *)*(u32 *)gPlayers + 0x58) & 1) != 0) {
        s32 t1 = *(u32 *)((u8 *)*(u32 *)data_ov000_02369b84 + 0x218);
        if (*(u32 *)((u8 *)t0 + 0x80) == t1) {
            return 1;
        }
    }
    return 0;
}
