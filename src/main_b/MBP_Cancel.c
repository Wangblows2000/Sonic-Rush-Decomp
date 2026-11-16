#include <nitro/types.h>

void MBP_ChangeState(u16 v);
void func_020186f8(void);
void MBP_Cancel(void) { MBP_ChangeState(6); func_020186f8(); }
