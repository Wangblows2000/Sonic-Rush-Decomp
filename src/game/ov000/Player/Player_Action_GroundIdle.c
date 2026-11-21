#include <nitro/types.h>

typedef struct UnkStruct_0234c3b4 UnkStruct_0234c3b4;
struct UnkStruct_0234c3b4 {
    u8 pad0[0xc];
    s32 unkC;
    u8 pad10[0x34 - 0x10];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5e];
    void *unk68;
    u8 pad6C[0x124 - 0x6c];
    s32 unk124;
    u8 pad128[0x328 - 0x128];
    s16 unk328;
    u8 pad32A[0x338 - 0x32a];
    void (*unk338)(UnkStruct_0234c3b4 *w);
};

void Player_State_Crouch(void);
void Player_State_GroundIdle(void);
void Player_SetAnimation(UnkStruct_0234c3b4 *w, s32 a);

void Player_Action_GroundIdle(UnkStruct_0234c3b4 *w) {
    s16 s;

    if (w->unk34 != 0 && w->unk338 != NULL) {
        w->unk338(w);
        return;
    }
    s = w->unk328;
    if (s < 0x30 && ((w->unk5C & 8) || (s != 0x16 && s != 0x39 && s != 0x3a && s != 0x3b)))
        w->unk5C |= 0x400;
    if (w->unk68 == Player_State_Crouch && w->unk124 == 0) {
        Player_SetAnimation(w, 4);
        w->unk5C &= ~4;
    } else {
        Player_SetAnimation(w, 0x3c);
        w->unk5C |= 4;
    }
    w->unk58 &= ~0x10;
    w->unk68 = Player_State_GroundIdle;
    w->unkC = 0;
}
