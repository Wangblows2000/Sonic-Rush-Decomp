#include <nitro/types.h>

typedef struct UnkStruct_02312c70 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02312c70;

void StageObj_SetAnimation(UnkStruct_02312c70 *s, s32 a);
void GmkLandAA_State_Decelerate(void);

void GmkLandAA_Action_Init(UnkStruct_02312c70 *s) {
    StageObj_SetAnimation(s, 2);
    s->func = GmkLandAA_State_Decelerate;
}
