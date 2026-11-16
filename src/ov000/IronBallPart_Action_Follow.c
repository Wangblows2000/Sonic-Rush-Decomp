#include <nitro/types.h>

typedef struct UnkStruct_0230ccd4 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x68 - 0x10];
    void (*func)(void);
} UnkStruct_0230ccd4;

void StageObj_SetAnimation(UnkStruct_0230ccd4 *s, s32 a);
void IronBallPart_State_FollowParent(void);

void IronBallPart_Action_Follow(UnkStruct_0230ccd4 *s) {
    StageObj_SetAnimation(s, 1);
    s->func = IronBallPart_State_FollowParent;
    s->unkC = 0;
}
