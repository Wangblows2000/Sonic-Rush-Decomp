#include <nitro/types.h>

typedef struct UnkStruct_022c5fbc_3 {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022c5fbc_3;

typedef struct UnkStruct_022c5fbc_2 {
    u8 pad[0x210];
    UnkStruct_022c5fbc_3 *unk210;
} UnkStruct_022c5fbc_2;

typedef struct UnkStruct_022c5fbc {
    u8 pad[0x10];
    s32 unk10;
    u8 pad1[0x68 - 0x14];
    void (*func)(void);
} UnkStruct_022c5fbc;

extern UnkStruct_022c5fbc_2 *gBoss1;

void Boss1Hammer_State_Unk_ov000_022c5fb8(void);

void Boss1Hammer_State_Unk_ov000_022c5fbc(UnkStruct_022c5fbc *s) {
    gBoss1->unk210->unk10 = s->unk10;
    s->func = Boss1Hammer_State_Unk_ov000_022c5fb8;
}
