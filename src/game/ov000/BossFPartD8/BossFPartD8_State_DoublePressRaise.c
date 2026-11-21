#include <nitro/types.h>

typedef struct UnkStruct_022f33d0 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x510];
    u8 unk57C;
} UnkStruct_022f33d0;

void BossFPartD8_TrackPlayerX_Far(UnkStruct_022f33d0 *s);
void BossFPartD8_State_DoublePressSetTrack(void);

void BossFPartD8_State_DoublePressRaise(UnkStruct_022f33d0 *s) {
    BossFPartD8_TrackPlayerX_Far(s);
    if (s->unk57C & 2)
        s->func = BossFPartD8_State_DoublePressSetTrack;
}
