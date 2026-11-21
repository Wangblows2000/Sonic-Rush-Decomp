/* templates.py: lifted from the ROM instruction stream, verified by match.py. */
#include <nitro/types.h>

extern u8 gExtraBossRingWork[];
s32 ExtraBoss_SpawnFromScrollTable();

void ExtraBoss_UpdateSpawnWork(void) {
    s32 t0 = (*(u32 *)((u8 *)*(u32 *)gExtraBossRingWork + 0x94) & 1);
    if (t0 != 0) {
        ExtraBoss_SpawnFromScrollTable(t0);
    }
    s32 t1 = *(u32 *)((u8 *)*(u32 *)gExtraBossRingWork + 0x90);
    if (t1 == 0) {
        return;
    }
    ((s32 (*)())t1)(t1);
}
