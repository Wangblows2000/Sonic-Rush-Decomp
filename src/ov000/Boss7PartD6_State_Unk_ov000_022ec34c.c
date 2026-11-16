#include <nitro/types.h>

typedef struct UnkStruct_022ec34c {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad2[0x68 - 0x5e];
    void (*func)(void);
} UnkStruct_022ec34c;

void func_ov000_022ec2c8(void);

void Boss7PartD6_State_Unk_ov000_022ec34c(UnkStruct_022ec34c *s) {
    if (s->unk5C & 8)
        s->func = func_ov000_022ec2c8;
}
