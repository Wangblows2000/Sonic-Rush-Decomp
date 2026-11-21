#include <nitro/types.h>

typedef struct UnkStruct_022ddd20 {
    u8 pad[0x220];
    void (*func)(void);
    s32 unk224;
    u8 pad1[0x5ec];
    u8 unk814;
    u8 pad2[0x3db];
    s32 unkBF0;
} UnkStruct_022ddd20;

void Boss4_SubState_SlamTiltUp(void);

void Boss4_SubState_SlamWaitAnim(UnkStruct_022ddd20 *s) {
    if (!(s->unk814 & 2))
        return;
    s->func = Boss4_SubState_SlamTiltUp;
    s->unk224 = 0;
    s->unkBF0 = 1;
}
