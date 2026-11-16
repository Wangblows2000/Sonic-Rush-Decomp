#include <nitro/types.h>

typedef struct UnkStruct_0231bf68 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_0231bf68;

void WaterJet_Action_Prepare(UnkStruct_0231bf68 *s);

void WaterJet_State_Wait(UnkStruct_0231bf68 *s) {
    s->unkC--;
    if (s->unkC != 0)
        return;
    WaterJet_Action_Prepare(s);
}
