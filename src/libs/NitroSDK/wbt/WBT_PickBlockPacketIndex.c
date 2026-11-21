#include <nitro/types.h>

typedef struct UnkNode_02014320 {
    u32 id;
    s32 size;
    u8 pad8[0x20];
    struct UnkNode_02014320 *next; /* 0x28 */
} UnkNode_02014320;

extern s32 data_020779a0;
extern u32 data_020779a4;
extern s32 data_020779a8;

UnkNode_02014320 *WBT_GetBlockListHead(void);
s32 WBT_GetMyDataPacketSize(void);

s32 WBT_PickBlockPacketIndex(u32 id, s32 idx)
{
    s32 k;
    UnkNode_02014320 *n = WBT_GetBlockListHead();
    if (id < 1000) {
        return idx;
    }
    for (; n != NULL; n = n->next) {
        if (n->id == id) {
            if (id == data_020779a4) {
                k = 0;
                while (idx == data_020779a0 || idx == data_020779a8) {
                    s32 blk;
                    idx++;
                    blk = WBT_GetMyDataPacketSize();
                    if (idx > (n->size + blk - 1) / WBT_GetMyDataPacketSize() - 1) {
                        idx = 0;
                    }
                    if (++k > 2) {
                        break;
                    }
                }
            }
            data_020779a4 = id;
            data_020779a8 = data_020779a0;
            data_020779a0 = idx;
            return idx;
        }
    }
    return -1;
}
