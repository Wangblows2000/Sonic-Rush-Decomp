#include <nitro/types.h>

BOOL func_ov000_022da910(void);
void func_ov000_022dac60(void);

void Boss3_State_Unk_ov000_022d9c90(void) {
    if (func_ov000_022da910() == 0)
        return;
    func_ov000_022dac60();
}
