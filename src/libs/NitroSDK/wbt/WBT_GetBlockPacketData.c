#include <nitro/types.h>

typedef struct UnkNode_02014288 {
    u32 id;
    u8 pad4[0x24];
    struct UnkNode_02014288 *next; /* 0x28 */
    u8 *data;                      /* 0x2c */
} UnkNode_02014288;

UnkNode_02014288 *WBT_GetBlockListHead(void);
s32 WBT_GetMyDataPacketSize(void);

u8 *WBT_GetBlockPacketData(u32 id, s32 idx)
{
    UnkNode_02014288 *n = WBT_GetBlockListHead();
    if (id < 1000) {
        u32 i;
        i = 0;
    loop:
        if (n != NULL) {
            if (i == id) {
                return (u8 *)n + idx * WBT_GetMyDataPacketSize();
            }
            i++;
            n = n->next;
            goto loop;
        }
        return NULL;
    }
    while (n != NULL) {
        if (n->id == id) {
            u8 *data = n->data;
            return data + idx * WBT_GetMyDataPacketSize();
        }
        n = n->next;
    }
    return NULL;
}
