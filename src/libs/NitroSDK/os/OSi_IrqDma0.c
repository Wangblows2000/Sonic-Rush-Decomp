#include <nitro/types.h>

void OSi_IrqCallback(u32 n);
void OSi_IrqDma0(void) { OSi_IrqCallback(0); }
