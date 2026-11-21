#include <nitro/types.h>

void OSi_IrqCallback(u32 n);
void OSi_IrqTimer0(void) { OSi_IrqCallback(4); }
