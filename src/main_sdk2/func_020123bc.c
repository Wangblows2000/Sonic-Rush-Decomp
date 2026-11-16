#include <nitro/types.h>
#include <main_sdk2/wmds.h>

u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);
void WmDataSharingReceiveData(UnkDS *ds, u16 aid, const void *src);
s32 func_02011ff4(UnkDS *ds, BOOL a);

typedef struct {
    u16 apiid;
    u16 errcode;
    u16 state;
    u8 pad6[6];
    u16 *data;    /* 0xc */
    u16 length;   /* 0x10 */
    u16 aid;      /* 0x12 */
    u8 pad14[8];
    UnkDS *arg;   /* 0x1c */
} UnkPortRecvCb_020123bc;

void func_020123bc(UnkPortRecvCb_020123bc *cb)
{
    UnkDS *ds = cb->arg;
    if (ds == NULL) {
        return;
    }
    if (cb->errcode == 0) {
        switch (cb->state) {
        case 0x15:
            WmDataSharingReceiveData(ds, cb->aid, cb->data);
            func_02011ff4(ds, FALSE);
            break;
        case 7:
            func_02011ff4(ds, FALSE);
            break;
        case 9: {
            u32 bit = 1U << cb->aid;
            u32 e = OS_DisableInterrupts();
            u16 idx = ds->writeIdx;
            ds->buf[idx].bitmap0 &= ~bit;
            if (ds->doubleMode == 1) {
                u16 next = (idx + 1) & 3;
                ds->buf[next].bitmap0 &= ~bit;
            }
            OS_RestoreInterrupts(e);
            func_02011ff4(ds, FALSE);
            if (ds->doubleMode == 1) {
                func_02011ff4(ds, FALSE);
            }
            break;
        }
        }
    } else {
        ds->state = 5;
    }
}
