#include <nitro/types.h>

typedef struct UnkStruct_0231bec0 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_0231bec0;

void WaterJet_Action_Spout(UnkStruct_0231bec0 *s);

void WaterJet_State_Prepare(UnkStruct_0231bec0 *s) {
    s->unkC--;
    if (s->unkC != 0)
        return;
    WaterJet_Action_Spout(s);
}
