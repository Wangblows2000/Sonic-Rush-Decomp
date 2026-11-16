#include <nitro/types.h>

typedef struct UnkStruct_022e7bf4 {
    u8 pad[0x30];
    u16 unk30;
    u8 pad1[0x26];
    u32 unk58;
    u8 pad2[0x3e4];
    void (*func)(void);
} UnkStruct_022e7bf4;

void Boss7PartD5_MoveState_Unk_ov000_022e98d8(void);

void Boss7PartD5_MoveState_Unk_ov000_022e7bf4(UnkStruct_022e7bf4 *s) {
    if (!(s->unk58 & 1))
        return;
    s->unk30 = 0;
    s->unk58 &= ~0x10;
    s->func = Boss7PartD5_MoveState_Unk_ov000_022e98d8;
}
