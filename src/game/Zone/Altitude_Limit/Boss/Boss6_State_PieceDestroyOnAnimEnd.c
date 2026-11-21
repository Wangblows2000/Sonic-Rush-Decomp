#include <nitro/types.h>

typedef struct {
    u8 pad0[0xe4];
    void *unkE4[5];
    u8 padF8[0x10c - 0xf8];
    u8 unk10C[5];
} UnkStruct_022dfd30_c;

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u8 pad58[0x134 - 0x58];
    UnkStruct_022dfd30_c cam;
} UnkStruct_022dfd30;

void Boss6_State_PieceDestroyOnAnimEnd(UnkStruct_022dfd30 *p) {
    UnkStruct_022dfd30_c *c = &p->cam;
    s32 i;

    for (i = 0; i < 5; i++) {
        if (c->unkE4[i] != NULL && !(c->unk10C[i] & 2))
            return;
    }
    p->unk54 |= 4;
}
