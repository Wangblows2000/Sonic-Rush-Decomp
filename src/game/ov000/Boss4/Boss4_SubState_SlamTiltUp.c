#include <nitro/types.h>

typedef struct {
    u8 pad0[0x220];
    void *unk220;
    s32 unk224;
    u8 pad228[0xBBC - 0x228];
    s32 unkBBC;
    u8 padBC0[0xBEC - 0xBC0];
    s32 unkBEC;
} UnkStruct_022ddc44;

void Boss4_SubState_SlamHold(void);

void Boss4_SubState_SlamTiltUp(UnkStruct_022ddc44 *s) {
    s32 n;
    float t;

    if (s->unkBEC != 0)
        n = 0x14;
    else
        n = 0x32;
    t = 1.0f - (float)s->unk224 / (float)n;
    t = 1.0f - t * (t * t);
    s->unkBBC = -0x600;
    s->unkBBC = s->unkBBC + 8448.0f * t;
    if (s->unkBBC >= 0x1b00) {
        s->unkBBC = 0x1b00;
        s->unk220 = Boss4_SubState_SlamHold;
        s->unk224 = 0;
    }
}
