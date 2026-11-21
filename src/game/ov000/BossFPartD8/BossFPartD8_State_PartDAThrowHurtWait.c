#include <nitro/types.h>

typedef struct UnkStruct_022f24f8 {
    u8 pad[0x57c];
    u8 unk57C;
} UnkStruct_022f24f8;

void BossFPartD8_TriggerHandRide(void);

void BossFPartD8_State_PartDAThrowHurtWait(UnkStruct_022f24f8 *s) {
    if (!(s->unk57C & 2))
        return;
    BossFPartD8_TriggerHandRide();
}
