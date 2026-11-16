#include <nitro/types.h>

typedef struct UnkStruct_022e9454 {
    u8 pad[0x2e];
    s16 unk2E;
    u8 pad1[0x28];
    u32 unk58;
    u8 pad2[0x3e4];
    void (*func)(void);
} UnkStruct_022e9454;

void Boss7PartD5_MoveState_StartIdle(void);

void Boss7PartD5_MoveState_BackHop(UnkStruct_022e9454 *s) {
    s->unk2E = (s->unk2E * 0xe0) >> 8;
    if (s->unk58 & 1) {
        s->unk58 &= ~0x10;
        s->func = Boss7PartD5_MoveState_StartIdle;
    }
}
