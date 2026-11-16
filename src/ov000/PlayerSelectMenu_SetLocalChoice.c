#include <nitro/types.h>

u8 *WirelessSession_GetSendBuffer(void *p);

#pragma thumb on
void PlayerSelectMenu_SetLocalChoice(void *p, u8 v) {
    u8 *q = WirelessSession_GetSendBuffer(p);
    q[0] = v;
    q[1] = 1;
}
#pragma thumb reset
