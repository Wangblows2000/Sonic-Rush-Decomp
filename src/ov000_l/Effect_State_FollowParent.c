#include <ov000_b/stageobj.h>

void Effect_State_FollowParent(UnkStruct_EffObj *w) {
    if (w->unk88 != NULL) {
        w->unk10 = ((UnkStruct_EffObj *)w->unk88)->unk10;
        w->unk14 = ((UnkStruct_EffObj *)w->unk88)->unk14;
        w->unk18 = ((UnkStruct_EffObj *)w->unk88)->unk18;
        if (w->unk5C & 1)
            w->unk10 -= w->unk2E * 0x100;
        else
            w->unk10 += w->unk2E * 0x100;
        w->unk14 += w->unk30 * 0x100;
    }
    if (w->unk5C & 4) {
        if (w->unkC == 0)
            w->unk54 |= 4;
        else
            w->unkC--;
        return;
    }
    if (w->unk5C & 8) {
        if (w->unkC == 0)
            w->unk54 |= 4;
        else
            w->unkC--;
    }
}
