#include <nitro/types.h>

typedef struct UnkStruct_022fc418 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x5c - 0x10];
    u16 unk5C;
    u8 pad2[0x68 - 0x5e];
    void (*func)(void);
} UnkStruct_022fc418;

void Enemy_DisableCollidersSetAnim(UnkStruct_022fc418 *s, s32 a);
void Enemy_State_PopOnContact(void);

void Enemy_Action_StartPopOnContact(UnkStruct_022fc418 *s) {
    Enemy_DisableCollidersSetAnim(s, 3);
    s->unk5C |= 4;
    s->func = Enemy_State_PopOnContact;
    s->unkC = 0x10;
}
