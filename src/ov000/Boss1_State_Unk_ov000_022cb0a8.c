#include <nitro/types.h>

BOOL Boss1_Unk_ov000_022cc1c4(void);
void Boss1_Action_Unk_ov000_022cbfc8(void);

void Boss1_State_Unk_ov000_022cb0a8(void) {
    if (Boss1_Unk_ov000_022cc1c4() == 0)
        return;
    Boss1_Action_Unk_ov000_022cbfc8();
}
