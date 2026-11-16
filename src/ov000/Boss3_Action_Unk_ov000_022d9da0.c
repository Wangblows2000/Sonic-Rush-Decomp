#include <nitro/types.h>

typedef struct UnkStruct_022d9da0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d9da0;

extern s32 gBossStatus[];

void Boss3_State_Unk_ov000_022d9d74(void);

void Boss3_Action_Unk_ov000_022d9da0(UnkStruct_022d9da0 *s) {
    gBossStatus[1] = 0xf;
    s->func = Boss3_State_Unk_ov000_022d9d74;
}
