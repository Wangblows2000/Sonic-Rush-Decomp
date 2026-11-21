#include <nitro/types.h>

void MBP_ChangeState(u16 v);
BOOL MB_StartParent(void *p);
BOOL MBP_RegistFile(void *p);
void OS_Terminate(void);
void MBP_Start(void *a, void *b) { MBP_ChangeState(2); if (MB_StartParent(b)) { MBP_ChangeState(7); return; } if (MBP_RegistFile(a)) return; OS_Terminate(); }
