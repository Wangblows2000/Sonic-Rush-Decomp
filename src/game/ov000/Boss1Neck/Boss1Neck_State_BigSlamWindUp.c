#include <nitro/types.h>

typedef struct UnkStruct_022c9478 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x2c8];
    u8 unk334;
} UnkStruct_022c9478;

void Boss1Neck_TrackPlayerSlow(UnkStruct_022c9478 *s);
void Boss1Neck_State_BigSlamHold(void);

void Boss1Neck_State_BigSlamWindUp(UnkStruct_022c9478 *s) {
    Boss1Neck_TrackPlayerSlow(s);
    if (s->unk334 & 2)
        s->func = Boss1Neck_State_BigSlamHold;
}
