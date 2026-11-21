#include <nitro/types.h>

typedef struct UnkStruct_022d5744 {
    u8 pad[0x2e];
    u16 unk2E;
    u8 pad2[0x68 - 0x30];
    void (*func)(void);
} UnkStruct_022d5744;

void Boss3PartCD_State_Defeated(void);

void Boss3PartCD_Action_Defeat(UnkStruct_022d5744 *s) {
    s->unk2E = 0;
    s->func = Boss3PartCD_State_Defeated;
}
