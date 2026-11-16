#include <nitro/types.h>

typedef struct UnkStruct_022d9cbc {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d9cbc;

extern s32 gBossStatus[];

void Boss3_State_Unk_ov000_022d9c90(void);

void Boss3_Action_Unk_ov000_022d9cbc(UnkStruct_022d9cbc *s) {
    gBossStatus[1] = 0x10;
    s->func = Boss3_State_Unk_ov000_022d9c90;
}
