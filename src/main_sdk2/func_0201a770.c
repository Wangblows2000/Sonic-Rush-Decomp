#include <nitro/types.h>
#include <main_sdk2/blk2.h>

void MI_CpuCopy8(const void *src, void *dest, u32 size);
BOOL func_0201a660(u16 aid);
u8 *func_0201a6c0(UnkPacket_0201a770 *packet, u16 aid);

void *func_0201a770(const u8 *src, UnkPacket_0201a770 *pkt, u16 aid)
{
    const u8 *ret;

    pkt->type = src[0];
    switch (pkt->type) {
    case 7:
        if (func_0201a660(aid)) {
            return data_02080f44->data[aid - 1];
        }
        pkt->u.s.seq = src[1];
        if (pkt->u.s.seq > data_02080f48.count) {
            return NULL;
        }
        MI_CpuCopy8(src + 2, pkt->u.s.data, data_02080f48.size);
        ret = func_0201a6c0(pkt, aid);
        break;
    case 8:
        pkt->u.h.val = (u16)(src[1] & 0x00ff);
        pkt->u.h.val |= (src[2] << 8) & 0xff00;
        ret = src + 3;
        break;
    case 9:
        pkt->u.h.val = (u16)(src[1] & 0x00ff);
        pkt->u.h.val |= (src[2] << 8) & 0xff00;
        ret = src + 3;
        MI_CpuCopy8(ret, &pkt->u.h.val2, data_02080f48.size);
        ret += data_02080f48.size;
        break;
    default:
        return NULL;
    }
    return (void *)ret;
}
