#include <nitro/types.h>

typedef struct UnkStruct_022c8874 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x2c8];
    u8 unk334;
} UnkStruct_022c8874;

void Boss1Neck_Unk_ov000_022ca00c(UnkStruct_022c8874 *s);
void Boss1Neck_State_Unk_ov000_022c87d8(void);

void Boss1Neck_State_Unk_ov000_022c8874(UnkStruct_022c8874 *s) {
    Boss1Neck_Unk_ov000_022ca00c(s);
    if (s->unk334 & 2)
        s->func = Boss1Neck_State_Unk_ov000_022c87d8;
}
