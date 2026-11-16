#include <nitro/types.h>

typedef struct UnkStruct_022c5fa8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c5fa8;

void Boss1Hammer_State_Unk_ov000_022c5fa4(void);

void Boss1Hammer_Action_Unk_ov000_022c5fa8(UnkStruct_022c5fa8 *s) {
    s->func = Boss1Hammer_State_Unk_ov000_022c5fa4;
}
