#include <nitro/types.h>

typedef struct UnkStruct_022c7e68 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x334 - 0x6c];
    u8 unk334;
} UnkStruct_022c7e68;

void Boss1Neck_State_Unk_ov000_022c7dfc(void);

void Boss1Neck_State_Unk_ov000_022c7e68(UnkStruct_022c7e68 *s) {
    if (s->unk334 & 2)
        s->func = Boss1Neck_State_Unk_ov000_022c7dfc;
}
