#include <nitro/types.h>

typedef struct UnkStruct_022c8a6c {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x334 - 0x6c];
    u8 unk334;
} UnkStruct_022c8a6c;

void Boss1Neck_State_Unk_ov000_022c8a2c(void);

void Boss1Neck_State_Unk_ov000_022c8a6c(UnkStruct_022c8a6c *s) {
    if (s->unk334 & 2)
        s->func = Boss1Neck_State_Unk_ov000_022c8a2c;
}
