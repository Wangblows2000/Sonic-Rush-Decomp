#include <nitro/types.h>

typedef struct UnkStruct_02308e94 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02308e94;

void StageObj_SetAnimation(UnkStruct_02308e94 *s, s32 a);
void GmkWall_State_WaitRoomStart(void);

void GmkWall_Action_Init(UnkStruct_02308e94 *s) {
    StageObj_SetAnimation(s, 0);
    s->func = GmkWall_State_WaitRoomStart;
}
