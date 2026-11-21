#include <game/Stage/stageobj_alt.h>

UnkStruct_EffObj *FixMoji3D_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void EffDrownCount3D_State_FollowPlayer(void);

void EffDrownCount_Spawn3D(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
    UnkStruct_EffObj *w = FixMoji3D_Create(a, b, c, d, e, f);

    if (w != NULL)
        w->unk68 = EffDrownCount3D_State_FollowPlayer;
}
