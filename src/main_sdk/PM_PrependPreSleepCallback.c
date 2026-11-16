#include <nitro/types.h>

void PMi_PrependList(void *q, void *p);
extern u8 PMi_PreSleepCallbackList[];
void PM_PrependPreSleepCallback(void *p) { PMi_PrependList(PMi_PreSleepCallbackList, p); }
