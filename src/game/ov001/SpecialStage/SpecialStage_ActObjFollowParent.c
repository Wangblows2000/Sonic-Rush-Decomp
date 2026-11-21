#include <nitro/types.h>

typedef struct { s32 x, y, z; } UnkStruct_02306b18_v;
void SpecialStageActObj_Destroy(void *p);
void *SpecialStage_GetPlayer(void);
BOOL SpecialStageObj_IsInReach(void *a, void *p);

void SpecialStage_ActObjFollowParent(u8 *p) {
    u8 *q = *(u8 **)(p + 0xd4);
    if (q != NULL) {
        if (*(u16 *)(q + 0x62) & 1) {
            *(UnkStruct_02306b18_v *)(p + 0x20) = *(UnkStruct_02306b18_v *)(q + 0x20);
            *(s32 *)(p + 0x28) += 0x1000;
        } else {
            SpecialStageActObj_Destroy(p);
        }
        return;
    }
    if (!SpecialStageObj_IsInReach(SpecialStage_GetPlayer(), p)) {
        SpecialStageActObj_Destroy(p);
    }
}
