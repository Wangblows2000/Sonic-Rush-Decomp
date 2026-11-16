/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

s32 func_ov000_022f89a8();
extern u8 gPlayers[];
extern u8 gStageMapCollision[];
s32 func_ov000_022f8888();

void func_ov000_022f1acc(u8 *p0) {
    s32 t0 = func_ov000_022f89a8();
    if ((*(u8 *)((u8 *)p0 + 0x57c) & 2) == 0) {
        return;
    }
    s32 t1 = (*(u8 *)((u8 *)p0 + 0x57c) & 2);
    s32 v2 = t0;
    s32 v3 = t1;
    if (t0 == 0) {
        s32 t4 = *(u32 *)gPlayers;
        *(u32 *)((u8 *)gStageMapCollision + 0x24) = 0x340;
        *(u32 *)((u8 *)t4 + 0x10) = 0x33400;
        v2 = t4;
        v3 = 0x33400;
    }
    func_ov000_022f8888(v2, v3);
}
