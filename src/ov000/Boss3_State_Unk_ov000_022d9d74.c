#include <nitro/types.h>

BOOL Boss3_Unk_ov000_022da994(void);
void Boss3_Unk_ov000_022dac60(void);

void Boss3_State_Unk_ov000_022d9d74(void) {
    if (Boss3_Unk_ov000_022da994() == 0)
        return;
    Boss3_Unk_ov000_022dac60();
}
