#include <nitro/types.h>

void OSi_IrqCallback(u32 n);
void OSi_IrqDma3(void) { OSi_IrqCallback(3); }
