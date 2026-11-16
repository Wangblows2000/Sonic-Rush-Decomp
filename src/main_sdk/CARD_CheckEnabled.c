#include <nitro/types.h>

u32 CARD_IsEnabled(void);
void OS_Terminate(void);
void CARD_CheckEnabled(void) { if (CARD_IsEnabled() != 0) return; OS_Terminate(); }
