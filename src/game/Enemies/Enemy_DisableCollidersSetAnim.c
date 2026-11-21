#include <nitro/types.h>

typedef struct UnkStruct_022faf48 {
    u8 pad[0x13e];
    u16 unk13E;
    u8 pad1[0x16a - 0x140];
    u16 unk16A;
    u8 pad2[0x196 - 0x16c];
    u16 unk196;
} UnkStruct_022faf48;

void StageObj_SetAnimation(UnkStruct_022faf48 *s, s32 a);

void Enemy_DisableCollidersSetAnim(UnkStruct_022faf48 *s, s32 a) {
    s->unk13E &= ~4;
    s->unk16A &= ~4;
    s->unk196 &= ~4;
    StageObj_SetAnimation(s, a);
}
