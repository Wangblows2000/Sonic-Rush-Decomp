#include <nitro/types.h>

void Boss1Hammer_State_Defeated(void);

typedef struct { u8 unk0[0x68]; void (*callback)(void); } UnkStruct_ov000_022c5f6c;

void Boss1Hammer_Action_Defeat(UnkStruct_ov000_022c5f6c *s) {
    s->callback = Boss1Hammer_State_Defeated;
}
