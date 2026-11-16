#include <nitro/types.h>

BOOL Valve_DrawSprites(void *p);
void Valve_Action_Wait(void *p);

void Valve_State_End(void *p) {
    if (Valve_DrawSprites(p) == 0)
        return;
    Valve_Action_Wait(p);
}
