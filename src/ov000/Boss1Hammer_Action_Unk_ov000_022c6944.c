#include <nitro/types.h>

typedef struct UnkStruct_022c6944 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c6944;

void Boss1Hammer_State_Unk_ov000_022c6940(void);

void Boss1Hammer_Action_Unk_ov000_022c6944(UnkStruct_022c6944 *s) {
    s->func = Boss1Hammer_State_Unk_ov000_022c6940;
}
