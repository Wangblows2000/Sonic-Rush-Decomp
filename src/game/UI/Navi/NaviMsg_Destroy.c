#include <nitro/types.h>

extern void *gNaviMsgTask;
void Task_Destroy(void *p);
void NaviMsg_Destroy(void) { if (gNaviMsgTask == NULL) return; Task_Destroy(gNaviMsgTask); }
