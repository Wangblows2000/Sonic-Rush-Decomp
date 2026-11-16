#include <nitro/types.h>

typedef struct UnkStruct_0235650c {
    u8 pad[0xc];
    s32 unkC;
} UnkStruct_0235650c;

u8 *WirelessSession_GetSendBuffer(void);

#pragma thumb on
void PlayerSelectMenu_SetLocalQuit(UnkStruct_0235650c *s) {
    if (s->unkC == 1)
        WirelessSession_GetSendBuffer()[2] = 1;
}
#pragma thumb reset
