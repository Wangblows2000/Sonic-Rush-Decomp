#include <nitro/types.h>

typedef struct UnkStruct_022cb0d4 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cb0d4;

extern s32 gBossStatus[];

void Boss1_State_Unk_ov000_022cb0a8(void);

void Boss1_Action_Unk_ov000_022cb0d4(UnkStruct_022cb0d4 *s) {
    gBossStatus[1] = 2;
    s->func = Boss1_State_Unk_ov000_022cb0a8;
}
