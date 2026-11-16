#include <nitro/types.h>

void PMi_AppendList(void *q, void *p);
extern u8 PMi_PostSleepCallbackList[];
void PM_AppendPostSleepCallback(void *p) { PMi_AppendList(PMi_PostSleepCallbackList, p); }
