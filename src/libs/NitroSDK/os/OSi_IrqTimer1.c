#include <nitro/types.h>

void OSi_IrqCallback(u32 n);
void OSi_IrqTimer1(void) { OSi_IrqCallback(5); }
