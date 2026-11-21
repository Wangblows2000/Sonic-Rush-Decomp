#include <nitro/types.h>
#include <main_sdk2/wm.h>

typedef struct {
    void *scanBuf;
    u16 channel;
    u16 maxChannelTime;
    u8 bssid[6];
} UnkScanParam_020117b8;

typedef struct {
    u16 apiid;
    u16 channel;
    void *scanBuf;
    u16 maxChannelTime;
    u8 bssid[6];
} UnkWmCmd_020117b8;

s32 WM_StartScan(void *cb, UnkScanParam_020117b8 *param)
{
    UnkWmCmd_020117b8 *cmd;
    s32 r = WMi_CheckStateEx(3, 2, 3, 5);
    if (r != 0) {
        return r;
    }
    if (param == NULL) {
        return 6;
    }
    if (param->scanBuf == NULL) {
        return 6;
    }
    if (param->channel < 1 || param->channel > 14) {
        return 6;
    }
    WMi_SetCallbackTable(0xa, cb);
    cmd = (UnkWmCmd_020117b8 *)((UnkWmWork *)WMi_GetSystemWork())->unkC;
    cmd->apiid = 0xa;
    cmd->channel = param->channel;
    cmd->scanBuf = param->scanBuf;
    cmd->maxChannelTime = param->maxChannelTime;
    cmd->bssid[0] = param->bssid[0];
    cmd->bssid[1] = param->bssid[1];
    cmd->bssid[2] = param->bssid[2];
    cmd->bssid[3] = param->bssid[3];
    cmd->bssid[4] = param->bssid[4];
    cmd->bssid[5] = param->bssid[5];
    r = WMi_SendCommand(0xa, 0);
    if (r == 0) {
        r = 2;
    }
    return r;
}
