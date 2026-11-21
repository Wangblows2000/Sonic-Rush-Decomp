#include <nitro/types.h>

extern u8 *gExtraBossRingWork;
extern u8 data_ov001_02316cac[];
extern u8 data_ov001_02316b9c[];
void ExtraBoss_ClearSpawnLists(void);
u8 *ExtraBoss_GetWork(void);
void ExtraBoss_UpdateRings(void);

void ExtraBoss_ResetSpawnWork(void) {
    ExtraBoss_ClearSpawnLists();
    *(s32 *)(gExtraBossRingWork + 0x137c) = 0;
    *(s32 *)(gExtraBossRingWork + 0x1380) = *(s32 *)(gExtraBossRingWork + 0x137c);
    if (*(s32 *)(ExtraBoss_GetWork() + 0x40) == 4) {
        *(void **)(gExtraBossRingWork + 0x1384) = data_ov001_02316b9c;
    } else {
        *(void **)(gExtraBossRingWork + 0x1384) = data_ov001_02316cac;
    }
    *(void (**)(void))(gExtraBossRingWork + 0x90) = ExtraBoss_UpdateRings;
    *(u32 *)(gExtraBossRingWork + 0x94) |= 3;
}
