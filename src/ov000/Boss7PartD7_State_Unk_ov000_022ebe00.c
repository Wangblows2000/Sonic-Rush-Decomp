#include <nitro/types.h>

typedef struct UnkStruct_022ebe00 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(void);
    u8 pad2[0x20c - 0x6c];
    u8 unk20C[0x358 - 0x20c];
    s32 unk358;
    u8 pad3[0x364 - 0x358 - 4];
    s32 unk364;
} UnkStruct_022ebe00;

void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void Boss7PartD7_State_WaitAnimEnd_ov000_022ebdc4(void);

void Boss7PartD7_State_Unk_ov000_022ebe00(UnkStruct_022ebe00 *s) {
    void *p = s->unk20C;
    s->unk5C &= ~4;
    Model_SetAnimation(p, 0, s->unk358, 0, 0);
    Model_SetAnimation(p, 1, s->unk364, 0, 0);
    s->func = Boss7PartD7_State_WaitAnimEnd_ov000_022ebdc4;
}
