#include <nitro/types.h>

typedef struct UnkStruct_0232ce4c {
    u8 pad[0x8];
    s32 unk8;
    s32 unkC;
    u8 pad1[0x36];
    u16 unk46;
    u8 pad2[0x20];
    void (*func)(void);
    u8 pad3[0x20a];
    u16 unk276;
} UnkStruct_0232ce4c;

void EnemyRoomHud_State_Unk_ov000_0232cd18(void);

void EnemyRoomHud_Action_Unk_ov000_0232ce4c(UnkStruct_0232ce4c *s) {
    s->unk276 = 0;
    s->unkC = 0;
    s->unk8 = 0;
    s->unk46 = 0x100;
    s->func = EnemyRoomHud_State_Unk_ov000_0232cd18;
}
