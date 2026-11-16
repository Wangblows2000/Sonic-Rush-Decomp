#include <nitro/types.h>
#include <main_sdk2/wh.h>

extern u32 data_020779d4;

void MBi_ParentCallback(UnkCallback_wh *arg);
void MBi_CheckWmErrcode(u16 id, s32 result);
void func_0201a014(void);
s32 WM_SetParentParameter(void (*cb)(UnkCallback_wh *), u32 a);
s32 WM_SetBeaconIndication(void (*cb)(UnkCallback_wh *), u32 a);
s32 WMi_StartParentEx(void (*cb)(UnkCallback_wh *), u32 a);
s32 func_020114f0(u16 port, void (*cb)(void *), void *arg);
void WM_SetIndCallback(u32 a);
s32 func_02011dcc(void (*cb)(UnkCallback_wh *), void *recvBuf, u16 recvBufSize, void *sendBuf, u16 sendBufSize,
                  u16 mpFreq, u16 retry, BOOL a, BOOL b, BOOL c, BOOL d);
BOOL MBi_IsSendEnabled(void);
s32 WM_End(void (*cb)(UnkCallback_wh *));
void OS_SpinWait(u32 cycles);
void OS_Terminate(void);

void MBi_ParentCallback(UnkCallback_wh *arg)
{
    switch (arg->apiid) {
    case 0:
        if (arg->errcode != 0) {
            data_02080f18->callback(0x100, arg);
            return;
        }
        func_0201a014();
        return;
    case 0x1d:
        if (arg->errcode != 0) {
            data_02080f18->callback(0x100, arg);
            return;
        }
        MBi_CheckWmErrcode(7, WM_SetParentParameter(MBi_ParentCallback, (u32)data_02080f18));
        return;
    case 7:
        data_02080f18->callback(0x15, arg);
        MBi_CheckWmErrcode(0x19, WM_SetBeaconIndication(MBi_ParentCallback, 1));
        return;
    case 0x19:
        if (arg->errcode != 0) {
            data_02080f18->callback(0x100, arg);
            return;
        }
        MBi_CheckWmErrcode(8, WMi_StartParentEx(MBi_ParentCallback, data_020779d4));
        return;
    case 8:
        if (arg->errcode != 0) {
            data_02080f18->callback(0x100, arg);
            return;
        }
        switch (arg->state) {
        case 0:
            data_02080f18->unk52a = 0;
            data_02080f18->unk528 = 0;
            return;
        case 7:
            if (data_02080f18->unk526 == 1) {
                return;
            }
            data_02080f18->unk52a |= 1 << arg->aid;
            data_02080f18->callback(0, arg);
            if (data_02080f18->unk528 == 0 && data_02080f1c->unk131c == 0) {
                data_02080f1c->unk131c = 1;
                MBi_CheckWmErrcode(0xe, func_02011dcc(MBi_ParentCallback, data_02080f18->recvBuf, data_02080f18->recvBufSize,
                                                 data_02080f18->sendBuf, data_02080f18->sendBufSize,
                                                 data_02080f18->unk52c ? 0 : 1, 0, 0, 0, 1, 1));
                return;
            }
            if (!MBi_IsSendEnabled()) {
                return;
            }
            data_02080f18->callback(0x19, 0);
            return;
        case 9:
            data_02080f18->unk52a &= ~(1 << arg->aid);
            data_02080f18->callback(1, arg);
            return;
        case 2:
            if (data_02080f18->unk526 == 1) {
                return;
            }
            data_02080f18->callback(0x1c, arg);
            return;
        default:
            data_02080f18->callback(0x100, arg);
            return;
        }
    case 14:
        data_02080f1c->unk131c = 0;
        switch (arg->state4) {
        case 10:
            data_02080f18->unk528 = 1;
            data_02080f18->callback(0x19, 0);
            return;
        case 11:
            data_02080f18->callback(3, *(void **)&arg->state);
            return;
        default:
            data_02080f18->callback(0x100, arg);
            return;
        }
    case 15:
        if (data_02080f1c->unk74c8 != 0) {
            u32 cnt = 0;
            u32 i;
            for (i = 0; i < 15; i++) {
                if (data_02080f1c->unk14e8[i] != 0) {
                    cnt++;
                    if (cnt >= 2) {
                        break;
                    }
                }
            }
            if (cnt == 1) {
                OS_SpinWait(13000);
            }
        }
        data_02080f18->unk50c = 0;
        if (arg->errcode == 0) {
            data_02080f18->callback(2, arg);
            data_02080f18->callback(0x19, 0);
            return;
        } else if (arg->errcode == 10) {
            data_02080f18->callback(0x2a, arg);
            return;
        } else {
            data_02080f18->callback(0x13, arg);
            data_02080f18->callback(0x19, 0);
            return;
        }
    case 1:
        if (data_02080f1c->unk1320 == 0) {
            if (arg->errcode != 0) {
                data_02080f18->unk526 = 0;
                data_02080f18->callback(0x100, arg);
                return;
            }
            data_02080f18->unk52a = 0;
            data_02080f18->unk528 = 0;
            MBi_CheckWmErrcode(2, WM_End(MBi_ParentCallback));
            return;
        }
        func_020114f0(1, 0, 0);
        WM_SetIndCallback(0);
    case 2:
        if (arg->errcode != 0) {
            data_02080f18->unk526 = 0;
            data_02080f18->callback(0x100, arg);
            return;
        }
        data_02080f18->unk50d = 0;
        data_02080f1c->unk1316 = 0;
        data_02080f18->callback(0x11, arg);
        return;
    case 13:
        if (arg->errcode != 0) {
            return;
        }
        data_02080f18->unk52a &= ~arg->connAid;
        return;
    case 0x80:
        switch (arg->state4) {
        case 0x10:
            data_02080f18->callback(0x1d, arg);
            return;
        case 0x11:
            data_02080f18->callback(0x1f, arg);
            return;
        case 0x12:
            data_02080f18->callback(0x20, arg);
            return;
        case 0x13:
            data_02080f18->callback(0x21, arg);
            return;
        case 0x16:
            OS_Terminate();
            return;
        }
        break;
    default:
        data_02080f18->callback(0x100, arg);
        break;
    }
}
