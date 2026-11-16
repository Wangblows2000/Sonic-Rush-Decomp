#include <nitro/types.h>

BOOL Boss3_Unk_ov000_022da968(void);
void Boss3_Unk_ov000_022dac60(void);

void Boss3_State_Unk_ov000_022d9d28(void) {
    if (Boss3_Unk_ov000_022da968() == 0)
        return;
    Boss3_Unk_ov000_022dac60();
}
