#include <nitro/types.h>

typedef struct UnkStruct_022c7970 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x210 - 0x6c];
    u16 unk210;
} UnkStruct_022c7970;

void Boss1Neck_State_TrackPlayer(void);

void Boss1Neck_State_StartTrackPlayer(UnkStruct_022c7970 *s) {
    s->unk210 = 0;
    s->func = Boss1Neck_State_TrackPlayer;
}
