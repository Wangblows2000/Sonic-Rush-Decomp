#include <nitro/types.h>

typedef struct UnkStruct_022ebf44 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(void);
    u8 pad2[0x20c - 0x6c];
    u8 unk20C[0x350 - 0x20c];
    s32 unk350;
    u8 pad3[0x35c - 0x350 - 4];
    s32 unk35C;
} UnkStruct_022ebf44;

void Model_SetAnimation(void *p, s32 a, s32 b, s32 c, s32 d);
void Boss7PartD7_State_WaitAnimEnd_ov000_022ebf08(void);

void Boss7PartD7_State_StartAppear(UnkStruct_022ebf44 *s) {
    void *p = s->unk20C;
    s->unk5C &= ~0x24;
    Model_SetAnimation(p, 0, s->unk350, 0, 0);
    Model_SetAnimation(p, 1, s->unk35C, 0, 0);
    s->func = Boss7PartD7_State_WaitAnimEnd_ov000_022ebf08;
}
