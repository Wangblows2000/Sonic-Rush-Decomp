#include <nitro/types.h>

typedef struct UnkNode_0206c970 UnkNode_0206c970;
struct UnkNode_0206c970 {
    u32 hdr;
    void *data;
    UnkNode_0206c970 *next;
};

typedef struct {
    u8 pad0[8];
    UnkNode_0206c970 *head;
} UnkStruct_022c4f1c;

typedef struct {
    u8 magic;
    u8 code;
    u16 keys;
} UnkHdr_0206c970;

extern u8 gWirelessSendBuffer[];
extern UnkStruct_022c4f1c data_022c4f1c;
extern u8 data_0207cfb0;
extern u8 data_0207cfb4[];
extern u16 gPad;
extern u16 gWHPacketSize;

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void MIi_CpuClear16(u16 v, void *dst, u32 size);
void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void NetPacket_UpdateSlotMap(void);
u8 WH_GetCurrentAid(void);
u32 NetPacket_GetSize(UnkNode_0206c970 *n);

BOOL NetPacket_BuildSendBuffer(void) {
    UnkNode_0206c970 *n;
    UnkNode_0206c970 *old;
    u32 len;
    u8 *buf;
    u16 pos;
    UnkStruct_022c4f1c *list;
    UnkHdr_0206c970 h;

    pos = 0;
    buf = gWirelessSendBuffer;
    list = &data_022c4f1c;
    {
        vu32 zero = 0;
        n = list->head;
        MIi_CpuClear32(zero, buf, 0x200);
    }
    NetPacket_UpdateSlotMap();
    if (data_0207cfb0 == 0xff)
        data_0207cfb0 = WH_GetCurrentAid();
    h.magic = 0x55;
    h.code = data_0207cfb4[data_0207cfb0];
    h.keys = gPad;
    data_0207cfb0 = WH_GetCurrentAid();
    MIi_CpuCopy16(&h, buf, 4);
    pos += 4;
    for (;;) {
        if (n == NULL) {
            list->head = NULL;
            return FALSE;
        }
        if (pos + NetPacket_GetSize(n) > gWHPacketSize)
            return TRUE;
        MIi_CpuCopy16(n, buf + pos, 4);
        pos += 4;
        if (n->data != NULL) {
            len = NetPacket_GetSize(n);
            MIi_CpuCopy16(n->data, buf + pos, len - 4);
            pos += len - 4;
        }
        old = n;
        n = n->next;
        list->head = n;
        {
            vu16 zero = 0;
            MIi_CpuClear16(zero, old, sizeof(UnkNode_0206c970));
        }
    }
}
