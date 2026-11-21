#include <nitro/types.h>

typedef struct {
    u8 pad0[0x12];
    u16 unk12;
    u8 pad14[0xc];
    u32 unk20;
    s32 unk24;
} UnkWork_02042810;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02042810 *work;
} UnkTask_02042810;

extern UnkTask_02042810 *gWirelessMgrTask;
extern u8 gWirelessUserGameInfo[];

void MI_CpuCopy8(const void *src, void *dest, u32 size);
s32 WM_SetGameInfo(void (*cb)(void), const void *data, u16 size, u32 a, u32 b, u8 c);
BOOL WirelessMgr_IsGameInfoPending(UnkWork_02042810 *w);
void WirelessMgr_OnGameInfoSet(void);

void WirelessMgr_UpdateGameInfo(void *src, u32 size, BOOL flag) {
    UnkWork_02042810 *w;
    u8 f = 0;

    if (gWirelessMgrTask == NULL)
        return;
    w = gWirelessMgrTask->work;
    w->unk24 = 1;
    if (src != NULL && size != 0) {
        w->unk20 = size;
        MI_CpuCopy8(src, gWirelessUserGameInfo, size);
    }
    if (flag)
        f |= 1;
    if (WM_SetGameInfo(WirelessMgr_OnGameInfoSet, gWirelessUserGameInfo, w->unk20, 0x51, w->unk12, f) != 2)
        return;
    while (WirelessMgr_IsGameInfoPending(w))
        ;
}
