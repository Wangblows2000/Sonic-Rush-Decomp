#include <nitro/types.h>

void MBP_ChangeState(u16 v);
void MB_End(void);
void MBP_Cancel(void) { MBP_ChangeState(6); MB_End(); }
