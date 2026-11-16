#include <nitro/types.h>

typedef struct UnkStruct_0231c808 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x20];
    u16 unk30;
    u8 pad2[0x26];
    u32 unk58;
    u8 pad3[0xc];
    void (*func)(void);
} UnkStruct_0231c808;

void Mine_State_Drop(void);

void Mine_Action_Drop(UnkStruct_0231c808 *s) {
    s->func = Mine_State_Drop;
    s->unk30 = 0x60;
    s->unk58 &= ~0x2000;
    s->unkC = 0;
}
