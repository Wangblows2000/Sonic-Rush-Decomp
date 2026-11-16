#include <nitro/types.h>

typedef struct UnkStruct_022fc084 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x5c - 0x10];
    u16 unk5C;
    u8 pad2[0x68 - 0x5e];
    void (*func)(void);
} UnkStruct_022fc084;

void Enemy_DisableCollidersSetAnim(UnkStruct_022fc084 *s, s32 a);
void EggBishop_State_Unk_ov000_022fc038(void);

void EggBishop_Action_Unk_ov000_022fc084(UnkStruct_022fc084 *s) {
    Enemy_DisableCollidersSetAnim(s, 4);
    s->unk5C |= 4;
    s->func = EggBishop_State_Unk_ov000_022fc038;
    s->unkC = 0x10;
}
