#include <nitro/types.h>

typedef struct UnkStruct_022c696c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c696c;

void Boss1Hammer_State_Unk_ov000_022c6968(void);

void Boss1Hammer_Action_Unk_ov000_022c696c(UnkStruct_022c696c *s) {
    s->func = Boss1Hammer_State_Unk_ov000_022c6968;
}
