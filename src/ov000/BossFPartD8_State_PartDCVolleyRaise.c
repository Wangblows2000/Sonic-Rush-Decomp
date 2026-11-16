#include <nitro/types.h>

typedef struct UnkStruct_022f18cc {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x510];
    u8 unk57C;
} UnkStruct_022f18cc;

void BossFPartD8_TrackPlayerX_NearSlow(UnkStruct_022f18cc *s);
void BossFPartD8_State_PartDCVolleyStartDelay(void);

void BossFPartD8_State_PartDCVolleyRaise(UnkStruct_022f18cc *s) {
    BossFPartD8_TrackPlayerX_NearSlow(s);
    if (s->unk57C & 2)
        s->func = BossFPartD8_State_PartDCVolleyStartDelay;
}
