#include <nitro/types.h>

typedef struct UnkStruct_0230ecc8_2 {
    u8 pad[4];
    u16 unk4;
} UnkStruct_0230ecc8_2;

typedef struct UnkStruct_0230ecc8 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x1e0 - 0x6c];
    UnkStruct_0230ecc8_2 *unk1E0;
} UnkStruct_0230ecc8;

void StageObj_SetAnimation(UnkStruct_0230ecc8 *s, u8 a);
void JumpPedestal_State_WaitPlayer(void);

void JumpPedestal_Action_Init(UnkStruct_0230ecc8 *s) {
    StageObj_SetAnimation(s, s->unk1E0->unk4 & 3);
    s->func = JumpPedestal_State_WaitPlayer;
}
