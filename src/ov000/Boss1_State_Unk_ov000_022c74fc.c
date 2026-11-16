#include <nitro/types.h>

typedef struct UnkStruct_022c74fc {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x334 - 0x6c];
    u8 unk334;
} UnkStruct_022c74fc;

void Boss1_State_Unk_ov000_022c74f8(void);

void Boss1_State_Unk_ov000_022c74fc(UnkStruct_022c74fc *s) {
    if (s->unk334 & 2)
        s->func = Boss1_State_Unk_ov000_022c74f8;
}
