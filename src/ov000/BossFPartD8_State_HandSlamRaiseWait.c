#include <nitro/types.h>

typedef struct UnkStruct_022f3dac {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x510];
    u8 unk57C;
} UnkStruct_022f3dac;

void BossFPartD8_TrackPlayerX_Fast(UnkStruct_022f3dac *s);
void BossFPartD8_State_HandSlamNext(void);

void BossFPartD8_State_HandSlamRaiseWait(UnkStruct_022f3dac *s) {
    BossFPartD8_TrackPlayerX_Fast(s);
    if (s->unk57C & 2)
        s->func = BossFPartD8_State_HandSlamNext;
}
