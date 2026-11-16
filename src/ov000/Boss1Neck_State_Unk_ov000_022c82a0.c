#include <nitro/types.h>

typedef struct UnkStruct_022c82a0 UnkStruct_022c82a0;
struct UnkStruct_022c82a0 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022c82a0 *s);
    u8 pad1[0x20e - 0x6c];
    u16 unk20E;
};

void Boss1Neck_State_Unk_ov000_022c825c(UnkStruct_022c82a0 *s);

void Boss1Neck_State_Unk_ov000_022c82a0(UnkStruct_022c82a0 *s) {
    s->unk20E = 0;
    s->func = Boss1Neck_State_Unk_ov000_022c825c;
    s->func(s);
}
