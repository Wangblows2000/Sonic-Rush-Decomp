#include <nitro/types.h>

void OSi_IrqCallback(u32 n);
void OSi_IrqDma1(void) { OSi_IrqCallback(1); }
