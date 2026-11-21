#include <nitro/types.h>

typedef struct UnkStruct_022d7204 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x1ec];
    u16 unk258;
    u8 pad2[0xa];
    u16 unk264;
} UnkStruct_022d7204;

void Boss3PartCC_State_KickWindUp(void);

void Boss3PartCC_Action_StartKickBall(UnkStruct_022d7204 *s) {
    s->unk258 = 0;
    s->unk264 = 0;
    s->func = Boss3PartCC_State_KickWindUp;
}
