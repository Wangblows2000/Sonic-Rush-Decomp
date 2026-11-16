#include <nitro/types.h>

void WH_End(void);
void WirelessMgr_State_DownloadPlayWaitStop(void);
void WirelessMgr_State_DownloadPlayEndWH(void (**p)(void)) { WH_End(); *p = WirelessMgr_State_DownloadPlayWaitStop; }
