#include <nitro/types.h>

typedef struct UnkStruct_022c8b38 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x334 - 0x6c];
    u8 unk334;
} UnkStruct_022c8b38;

void Boss1Neck_State_RollAnim3(void);

void Boss1Neck_State_RollWaitAnim2(UnkStruct_022c8b38 *s) {
    if (s->unk334 & 2)
        s->func = Boss1Neck_State_RollAnim3;
}
