#include <nitro/types.h>

void CTRDGi_SendtoPxi(u32 a);
void OS_Terminate(void);
void CTRDG_TerminateForPulledOut(void) { CTRDGi_SendtoPxi(2); OS_Terminate(); }
