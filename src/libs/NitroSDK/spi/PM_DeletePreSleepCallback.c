#include <nitro/types.h>

void PMi_DeleteList(void *q, void *p);
extern u8 PMi_PreSleepCallbackList[];
void PM_DeletePreSleepCallback(void *p) { PMi_DeleteList(PMi_PreSleepCallbackList, p); }
