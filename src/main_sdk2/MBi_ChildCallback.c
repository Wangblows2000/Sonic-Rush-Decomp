#include <nitro/types.h>
#include <main_sdk2/wh.h>

extern u16 data_020779c0;
extern u16 data_020779c4;
extern u16 data_020779c8;
extern u16 data_020779cc;
extern u16 data_02080f10;

extern UnkScanParam_wh data_02080f20;

void MBi_ChildCallback(UnkCallback_wh *arg);
void func_02019744(void *arg);
s32 WM_SetLifeTime(void (*cb)(UnkCallback_wh *), u16 a, u16 b, u16 c, u16 d);
void MBi_CheckWmErrcode(u16 id, s32 result);
s32 WM_StartScan(void (*cb)(UnkCallback_wh *), UnkScanParam_wh *param);
void DC_InvalidateRange(void *p, u32 size);
void MI_DmaCopy16(u32 dma, const void *src, void *dst, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
BOOL func_0201a0f0(UnkScanParam_wh *param);
void MBi_CommEnd(void);
s32 func_020116a0(void (*cb)(UnkCallback_wh *), u32 a, u32 b, u32 c, u32 d);
s32 func_020114f0(u16 port, void (*cb)(void *), void *arg);
s32 func_02011dcc(void (*cb)(UnkCallback_wh *), void *recvBuf, u16 recvBufSize, void *sendBuf, u16 sendBufSize,
                  u16 mpFreq, u16 retry, BOOL a, BOOL b, BOOL c, BOOL d);
BOOL MBi_IsSendEnabled(void);
s32 WM_End(void (*cb)(UnkCallback_wh *));
void OS_Terminate(void);

void MBi_ChildCallback(UnkCallback_wh *arg)
{
    UnkWork_wh *work = data_02080f18;
    s32 i;
    UnkParentEntry_wh *parents;

    switch (arg->apiid) {
    case 0:
        if (arg->errcode != 0) {
            work->callback(0x100, arg);
            return;
        }
        work->callback(0x15, arg);
        MBi_CheckWmErrcode(0x1d, WM_SetLifeTime(MBi_ChildCallback, data_020779cc, data_020779c4, data_020779c0, data_020779c8));
        return;
    case 0x1d:
        if (arg->errcode != 0) {
            work->callback(0x100, arg);
            return;
        }
        data_02080f20.scanBuf = work->scanBuf;
        if (data_02080f20.channel == 0) {
            data_02080f20.channel = 1;
        }
        if (data_02080f20.maxChannelTime == 0) {
            data_02080f20.maxChannelTime = 200;
        }
        data_02080f20.bssid[0] = 0xff;
        data_02080f20.bssid[1] = 0xff;
        data_02080f20.bssid[2] = 0xff;
        data_02080f20.bssid[3] = 0xff;
        data_02080f20.bssid[4] = 0xff;
        data_02080f20.bssid[5] = 0xff;
        work->unk5e4 = 1;
        work->unk5e8 = 1;
        MBi_CheckWmErrcode(0xa, WM_StartScan(MBi_ChildCallback, &data_02080f20));
        return;
    case 10:
        if (arg->errcode != 0) {
            work->callback(0x100, arg);
            return;
        }
        switch (arg->state) {
        case 3:
            return;
        case 5:
            parents = work->parents;
            for (i = 0; i < work->parentCount; i++) {
                if (arg->mac[0] == parents[i].scan.mac[0] && arg->mac[1] == parents[i].scan.mac[1] &&
                    arg->mac[2] == parents[i].scan.mac[2] && arg->mac[3] == parents[i].scan.mac[3] &&
                    arg->mac[4] == parents[i].scan.mac[4] && arg->mac[5] == parents[i].scan.mac[5]) {
                    parents[i].scan.gameInfoLength = arg->gameInfoLength;
                    parents[i].scan.gameInfo = arg->gameInfo;
                    DC_InvalidateRange(&work->parents[i], 0xc0);
                    MI_DmaCopy16(data_02080f10, work->scanBuf, &work->parents[i], 0xc0);
                    work->unk5ec = i;
                    goto found;
                }
            }
            if (i < 16) {
                work->parentCount = i + 1;
                MIi_CpuCopy16(arg, &parents[i].scan, sizeof(UnkCallback_wh));
                DC_InvalidateRange(&work->parents[i], 0xc0);
                MI_DmaCopy16(data_02080f10, work->scanBuf, &work->parents[i], 0xc0);
                work->unk5ec = i;
            }
        found:
            work->callback(4, arg);
            if (work->unk5e4 == 0) {
                return;
            }
            if (work->unk5e8 != 0 && !func_0201a0f0(&data_02080f20)) {
                MBi_CommEnd();
            }
            MBi_CheckWmErrcode(0xa, WM_StartScan(MBi_ChildCallback, &data_02080f20));
            return;
        case 4:
            work->callback(5, arg);
            if (work->unk5e4 == 0) {
                return;
            }
            if (work->unk5e8 != 0 && !func_0201a0f0(&data_02080f20)) {
                MBi_CommEnd();
            }
            MBi_CheckWmErrcode(0xa, WM_StartScan(MBi_ChildCallback, &data_02080f20));
            return;
        default:
            work->callback(0x100, arg);
            return;
        }
    case 11:
        if (arg->errcode != 0) {
            work->callback(0x100, arg);
            return;
        }
        MBi_CheckWmErrcode(0xc, func_020116a0(MBi_ChildCallback, work->unk520, 0, 1, 0));
        return;
    case 12:
        if (arg->errcode != 0) {
            work->parentCount = 0;
            work->callback(0xb, arg);
            return;
        }
        switch (arg->state) {
        case 6:
            work->unk52a = 0;
            work->unk528 = 1;
            return;
        case 7:
            work->aid = arg->connAid;
            work->callback(6, arg);
            work->unk52a = 1;
            if (func_020114f0(1, func_02019744, 0) != 0) {
                return;
            }
            MBi_CheckWmErrcode(0xe, func_02011dcc(MBi_ChildCallback, work->recvBuf, work->recvBufSize, work->sendBuf,
                                             work->sendBufSize, work->unk52c == 0, 0, 0, 0, 1, 1));
            return;
        case 9:
            work->callback(0xa, arg);
            work->unk52a = 0;
            work->unk528 = 0;
            return;
        default:
            work->callback(0x100, arg);
            return;
        }
    case 14:
        switch (arg->state4) {
        case 10:
            work->unk528 = 1;
            if (!MBi_IsSendEnabled()) {
                return;
            }
            work->callback(0x19, 0);
            return;
        case 12:
            return;
        case 13:
            return;
        default:
            work->callback(0x100, arg);
            return;
        }
    case 15:
        work->unk50c = 0;
        if (arg->errcode == 0) {
            work->callback(8, arg);
        } else if (arg->errcode == 9) {
            work->callback(0x29, arg);
        } else {
            work->callback(0x12, arg);
        }
        work->callback(0x19, 0);
        return;
    case 1:
        if (arg->errcode != 0) {
            work->unk526 = 0;
            work->callback(0x100, arg);
            return;
        }
        work->unk52a = 0;
        data_02080f18->unk528 = 0;
        MBi_CheckWmErrcode(2, WM_End(MBi_ChildCallback));
        return;
    case 2:
        if (arg->errcode != 0) {
            work->unk526 = 0;
            work->callback(0x100, arg);
            return;
        }
        work->unk50d = 0;
        data_02080f1c->unk1316 = 0;
        work->callback(0x11, arg);
        return;
    case 21:
        if (!MBi_IsSendEnabled()) {
            return;
        }
        work->callback(0x19, 0);
        return;
    case 0x80:
        if (arg->state4 != 0x16) {
            return;
        }
        OS_Terminate();
        return;
    default:
        work->callback(0x100, arg);
        return;
    }
}
