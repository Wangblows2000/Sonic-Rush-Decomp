#include <nitro/types.h>
#include <main_sdk2/wmds.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);
void WmDataSharingReceiveCallback_Parent(void *cb);
void WmDataSharingReceiveCallback_Child(void *cb);

typedef struct {
    u16 apiid;
    u16 errcode;
    u8 pad4[6];
    u16 port;     /* 0xa */
    u8 padc[0xe];
    u16 seq;      /* 0x1a */
    u8 pad1c[4];
    UnkDS *arg;   /* 0x20 */
} UnkPortSendCb;

void WmDataSharingSetDataCallback(UnkPortSendCb *cb)
{
    u8 *sys = WMi_GetSystemWork();
    u8 *p = ((UnkWmWork *)sys)->unk4;
    void *portCb = ((void **)(sys + 0xc0))[cb->port];
    UnkDS *ds = ((UnkDS **)(sys + 0x100))[cb->port];
    u16 aid;

    if (portCb != WmDataSharingReceiveCallback_Parent && portCb != WmDataSharingReceiveCallback_Child) {
        return;
    }
    if (ds == NULL) {
        return;
    }
    if (ds != cb->arg) {
        return;
    }
    DC_InvalidateRange(p + 0x184, 2);
    aid = *(u16 *)(p + 0x184);
    if (cb->errcode == 0) {
        if (aid != 0) {
            return;
        }
        ds->seq[ds->readIdx] = (s32)cb->seq >> 1;
        ds->readIdx = (ds->readIdx + 1) & 3;
    } else if (cb->errcode == 10) {
        if (aid != 0) {
            ds->readIdx = (ds->readIdx + 3) & 3;
        }
        ds->state = 4;
    } else {
        ds->state = 5;
    }
}
