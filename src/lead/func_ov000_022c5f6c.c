#include <nitro/types.h>

void Boss1Hammer_State_Unk_ov000_022c5f68(void);

typedef struct { u8 unk0[0x68]; void (*callback)(void); } UnkStruct_ov000_022c5f6c;

void func_ov000_022c5f6c(UnkStruct_ov000_022c5f6c *s) {
    s->callback = Boss1Hammer_State_Unk_ov000_022c5f68;
}
