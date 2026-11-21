#include <nitro/types.h>

void OSi_IrqCallback(u32 n);
void OSi_IrqDma2(void) { OSi_IrqCallback(2); }
