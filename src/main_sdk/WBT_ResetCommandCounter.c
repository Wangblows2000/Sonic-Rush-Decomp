#include <nitro/types.h>

extern u8 data_02080604;
void WBT_ResetCommandCounter(void) { data_02080604 = 0; }
