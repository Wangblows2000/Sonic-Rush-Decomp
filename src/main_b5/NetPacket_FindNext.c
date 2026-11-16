#include <nitro/types.h>

extern u8 *gWirelessRecvBuffers[];
extern u8 data_0207cfb4[];
extern u8 *data_022c4f14;

void NetPacket_UpdateSlotMap(void);
s32 NetPacket_GetSize(u8 *p);

u8 *NetPacket_FindNext(u8 type, s32 sec) {
    u8 *start;
    u8 *p;

    if (*gWirelessRecvBuffers[0] != 0x55)
        return NULL;
    NetPacket_UpdateSlotMap();
    start = gWirelessRecvBuffers[data_0207cfb4[sec]];
    p = data_022c4f14;
    if (start <= p && gWirelessRecvBuffers[data_0207cfb4[sec] + 1] > p) {
        p = data_022c4f14 += NetPacket_GetSize(p);
    } else {
        data_022c4f14 = start;
        p = data_022c4f14 += 4;
    }
    for (;;) {
        if (gWirelessRecvBuffers[data_0207cfb4[sec] + 1] <= p) {
            data_022c4f14 = NULL;
            return NULL;
        }
        if (*p == type)
            return p;
        if (*p == 0)
            return NULL;
        p = data_022c4f14 += NetPacket_GetSize(p);
    }
}
