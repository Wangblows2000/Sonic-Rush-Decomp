#include <nitro/types.h>
#include <NitroSDK/blk2.h>

u8 *MBi_MakeParentSendBuffer(const UnkPacket_0201a770 *pkt, u8 *dst)
{
    u8 *p = dst;
    *p++ = pkt->type;
    switch (pkt->type) {
    case 1:
    case 2:
    case 3:
        break;
    case 4:
        *p++ = pkt->u.h.val;
        *p++ = (pkt->u.h.val & 0xff00) >> 8;
        *p++ = pkt->u.h.val2;
        *p++ = (pkt->u.h.val2 & 0xff00) >> 8;
        break;
    case 5:
    case 6:
        break;
    case 0:
    default:
        return NULL;
    }
    return p;
}
