#include <nitro/types.h>

typedef struct UnkStruct_02315a48 {
    u8 pad[0x58];
    u32 unk58;
    u8 pad1[0xc];
    void (*func)(void);
} UnkStruct_02315a48;

void Saucer_State_Idle(void);

void Saucer_Action_Idle(UnkStruct_02315a48 *s) {
    s->func = Saucer_State_Idle;
    s->unk58 |= 0x10;
}
