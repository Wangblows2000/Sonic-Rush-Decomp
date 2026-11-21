#include <nitro/types.h>

typedef struct UnkStruct_022f669c {
    u8 pad[0x30];
    s16 unk30;
    u8 pad1[0x36];
    void (*func)(void);
} UnkStruct_022f669c;

void BossFPartDE_State_StartTrack(void);

void BossFPartDE_State_Land(UnkStruct_022f669c *s) {
    s->unk30 = (s->unk30 * 0xd0) >> 8;
    if (s->unk30 <= 0x30) {
        s->unk30 = 0;
        s->func = BossFPartDE_State_StartTrack;
    }
}
