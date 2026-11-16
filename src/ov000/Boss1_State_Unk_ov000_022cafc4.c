#include <nitro/types.h>

BOOL func_ov000_022cc140(void);
void Boss1_Action_Unk_ov000_022cbfc8(void);

void Boss1_State_Unk_ov000_022cafc4(void) {
    if (func_ov000_022cc140() == 0)
        return;
    Boss1_Action_Unk_ov000_022cbfc8();
}
