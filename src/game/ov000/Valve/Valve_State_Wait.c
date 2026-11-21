#include <nitro/types.h>

typedef struct UnkStruct_0231aed8 {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_0231aed8;

void Valve_Action_Puff(UnkStruct_0231aed8 *s);

void Valve_State_Wait(UnkStruct_0231aed8 *s) {
    s->unkC--;
    if (s->unkC != 0)
        return;
    Valve_Action_Puff(s);
}
