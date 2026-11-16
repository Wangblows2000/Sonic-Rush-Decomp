#include <nitro/types.h>

typedef struct { u8 pad[8]; u16 x; u16 y; u8 pad2[4]; u32 flags; } UnkStruct_022b65b4;
extern UnkStruct_022b65b4 gTouchState;
void **SpecialStage_GetGimmickManager(void);
void *SpecialStageGimmick_FindTouched(void *p, void *a, s32 x, s32 y);
void SpecialStagePlayer_ChangeState(void *p, s32 v);

BOOL SpecialStagePlayer_CheckTouchTarget(u8 *p) {
    if (*(s32 *)(p + 0x114) == 0) {
        return FALSE;
    }
    if (gTouchState.flags & 0x40) {
        *(void **)(p + 0x118) = SpecialStageGimmick_FindTouched(p, *SpecialStage_GetGimmickManager(), gTouchState.x, gTouchState.y);
        if (*(void **)(p + 0x118) != NULL) {
            SpecialStagePlayer_ChangeState(p, 0x12);
            return TRUE;
        }
    }
    return FALSE;
}
