#include <nitro/types.h>
#include <main_sdk2/wmds.h>
#include <main_sdk2/wm.h>

void DC_InvalidateRange(void *p, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);

typedef struct {
    u16 apiid;
    u16 errcode;
    u16 state;
    u8 pad6[6];
    u16 *data;    /* 0xc */
    u16 length;   /* 0x10 */
    u8 pad12[8];
    u16 seq;      /* 0x1a */
    UnkDS *arg;   /* 0x1c */
} UnkPortRecvCb;

void func_0201228c(UnkPortRecvCb *cb)
{
    UnkWmWork *w = func_02010b70();
    u8 *p = w->unk4;
    UnkDS *ds = cb->arg;
    u16 length;
    u16 bitmap;
    u16 *data;
    u16 aid;

    if (ds == NULL) {
        return;
    }
    if (cb->errcode == 0) {
        switch (cb->state) {
        case 7:
            return;
        case 9:
            return;
        case 0x15:
            break;
        default:
            return;
        }
        data = cb->data;
        bitmap = *data;
        length = cb->length;
        DC_InvalidateRange(p + 0x184, 2);
        aid = *(u16 *)(p + 0x184);
        if (length != ds->setLength) {
            if (length > 0x200) {
                length = 0x200;
            }
        }
        if (length < 4) {
            return;
        }
        if (!(bitmap & (1 << aid))) {
            return;
        }
        MIi_CpuCopy16(data, &ds->buf[ds->writeIdx], length);
        ds->seq[ds->writeIdx] = (s32)cb->seq >> 1;
        ds->writeIdx = (ds->writeIdx + 1) & 3;
    } else {
        ds->state = 5;
    }
}
