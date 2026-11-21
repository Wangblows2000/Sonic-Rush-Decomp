#include <nitro/types.h>

extern vu16 data_027e0000;

void OS_InitIrqTable(void) { data_027e0000 = 0; }
