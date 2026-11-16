#include <nitro/types.h>

typedef struct UnkStruct_023548a4 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_023548a4;

void Player_Action_BendStickLaunch(UnkStruct_023548a4 *s);

void Player_State_BendStick(UnkStruct_023548a4 *s) {
    if (s->unkC != 0) {
        s->unkC--;
        return;
    }
    Player_Action_BendStickLaunch(s);
}
