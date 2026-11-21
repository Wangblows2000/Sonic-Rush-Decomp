#include <game/Stage/stageobj_alt.h>

void Math_RotateVec2S16(s16 *x, s16 *y, u8 a);
void Player_State_Grind(void);

void EffGrind_State_FollowParent(UnkStruct_EffObj *w) {
    s16 x;
    s16 y;

    if (w->unk88 == NULL)
        return;
    x = w->unk2E;
    y = w->unk30;
    Math_RotateVec2S16(&x, &y, ((UnkStruct_EffObj *)w->unk88)->unk5 + ((UnkStruct_EffObj *)w->unk88)->unk1);
    w->unk10 = ((UnkStruct_EffObj *)w->unk88)->unk10 + (x << 8);
    w->unk14 = ((UnkStruct_EffObj *)w->unk88)->unk14 + (y << 8);
    if (((UnkStruct_EffObj *)w->unk88)->unk68 != Player_State_Grind)
        w->unk54 |= 4;
}
