#include <nitro/types.h>

typedef struct UnkStruct_022c8ad4 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0xa];
    void (*func)(void);
    u8 pad2[0x2c8];
    u8 unk334;
} UnkStruct_022c8ad4;

void Boss1Neck_State_RollWaitHammer(void);

void Boss1Neck_State_RollWaitAnim3(UnkStruct_022c8ad4 *s) {
    if (s->unk334 & 2) {
        s->unk5C |= 0x20;
        s->func = Boss1Neck_State_RollWaitHammer;
    }
}
