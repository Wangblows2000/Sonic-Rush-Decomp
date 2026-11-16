#include <nitro/types.h>

typedef struct UnkStruct_022d9c70 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d9c70;

extern s32 gBossStatus[];

void Boss3_State_Unk_ov000_022d9c44(void);

void Boss3_Action_Hurt(UnkStruct_022d9c70 *s) {
    gBossStatus[1] = 0x12;
    s->func = Boss3_State_Unk_ov000_022d9c44;
}
