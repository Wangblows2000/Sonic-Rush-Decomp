#include <nitro/types.h>

typedef struct UnkStruct_022c8874 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x2c8];
    u8 unk334;
} UnkStruct_022c8874;

void Boss1Neck_TrackPlayerMedium(UnkStruct_022c8874 *s);
void Boss1Neck_State_TripleSlamHold(void);

void Boss1Neck_State_TripleSlamWindUp(UnkStruct_022c8874 *s) {
    Boss1Neck_TrackPlayerMedium(s);
    if (s->unk334 & 2)
        s->func = Boss1Neck_State_TripleSlamHold;
}
