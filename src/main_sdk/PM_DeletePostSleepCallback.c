#include <nitro/types.h>

void PMi_DeleteList(void *q, void *p);
extern u8 PMi_PostSleepCallbackList[];
void PM_DeletePostSleepCallback(void *p) { PMi_DeleteList(PMi_PostSleepCallbackList, p); }
