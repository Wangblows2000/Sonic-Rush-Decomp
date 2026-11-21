#include <nitro/types.h>

typedef struct UnkStruct_02358524 {
    u8 pad[8];
    void (*unk8)(void);
} UnkStruct_02358524;

extern UnkStruct_02358524 *gTaskCurrent;

u8 *WirelessSession_GetSendBuffer(void);
void PlayerSelectMenu_State_Sync(void);

#pragma thumb on
void PlayerSelectMenu_State_StartSync(void) {
    WirelessSession_GetSendBuffer()[3] = 0;
    gTaskCurrent->unk8 = PlayerSelectMenu_State_Sync;
}
#pragma thumb reset
