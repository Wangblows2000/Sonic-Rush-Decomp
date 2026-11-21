#include <nitro/types.h>

typedef struct UnkStruct_023277ac UnkStruct_023277ac;
struct UnkStruct_023277ac {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1[0x88 - 0x1c];
    UnkStruct_023277ac *unk88;
};

void EffBoostA_3D_State_FollowParent(UnkStruct_023277ac *s) {
    s->unk10 = s->unk88->unk10;
    s->unk14 = s->unk88->unk14;
    s->unk18 = s->unk88->unk18;
}
