/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gGameState[];
extern u8 data_ov000_02368934[];
extern u8 data_ov000_0236ac18[];

void ZoneFx_Update(void) {
    s32 t0 = *(u32 *)((u8 *)(*(u8 *)((u8 *)gGameState + 0x1c) * 0xc + (s32)data_ov000_02368934) + (*(u8 *)((u8 *)gGameState + 0x1d) << 2));
    s32 t1 = *(u8 *)((u8 *)gGameState + 0x1c);
    if (t0 == 0) {
        return;
    }
    ((s32 (*)())t0)(*(u32 *)data_ov000_0236ac18, t0, data_ov000_02368934, t1);
}
