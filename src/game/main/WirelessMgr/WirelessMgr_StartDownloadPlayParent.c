#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    u8 *work;
} UnkTask_02042bbc;

extern UnkTask_02042bbc *gWirelessMgrTask;

UnkTask_02042bbc *Task_Create(void (*fn)(void), void (*fn2)(void), s32 a, s32 b, s32 c, s32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void MIi_CpuCopy16(const void *src, void *dest, u32 size);
void WH_Initialize(void);
void WH_SetGgid(s32 a);
void WirelessMgr_MainDownloadPlay(void);
void WirelessMgr_DestructorDownloadPlay(void);
void WirelessMgr_State_BeginMeasureChannel(void);

void WirelessMgr_StartDownloadPlayParent(const void *src, u32 v) {
    u8 *w;
    vu16 z;

    gWirelessMgrTask = Task_Create(WirelessMgr_MainDownloadPlay, WirelessMgr_DestructorDownloadPlay, 0, 1, 0xfffe, 0x1cf4);
    w = gWirelessMgrTask->work;
    z = 0;
    MIi_CpuClear16(z, w, 0x1cf4);
    *(void (**)(void))w = WirelessMgr_State_BeginMeasureChannel;
    *(s32 *)(w + 4) = 3;
    *(s32 *)(w + 8) = 1;
    MIi_CpuCopy16(src, w + 0x1af4, 0x1c);
    *(u32 *)(w + 0x1b10) = v;
    *(u32 *)(w + 0x1b08) = 0x51;
    WH_Initialize();
    WH_SetGgid(0x51);
}
