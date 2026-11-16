#include <nitro/types.h>

BOOL Boss1_Unk_ov000_022cc198(void);
void Boss1_Action_Unk_ov000_022cbfc8(void);

void Boss1_State_Unk_ov000_022cb05c(void) {
    if (Boss1_Unk_ov000_022cc198() == 0)
        return;
    Boss1_Action_Unk_ov000_022cbfc8();
}
