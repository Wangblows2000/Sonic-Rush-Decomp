#include <nitro/types.h>

typedef struct UnkStruct_022d4bc8 {
    u8 pad[0x24];
    s32 unk24;
    u8 pad1[0x92 - 0x28];
    u8 unk92;
} UnkStruct_022d4bc8;

extern s32 data_ov000_02365104;

BOOL Boss2_StoreNodeMatrix(UnkStruct_022d4bc8 *s, s32 a, s32 b);

void Boss2_NodeCallback_Head(UnkStruct_022d4bc8 *s) {
    if (Boss2_StoreNodeMatrix(s, 0x1a, data_ov000_02365104)) {
        s->unk24 = 0;
        s->unk92 = 0;
    }
}
