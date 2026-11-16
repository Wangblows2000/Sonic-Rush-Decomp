#include <nitro/types.h>

typedef struct UnkStruct_022d15f4 {
    u8 pad[0xba8];
    u8 unkBA8;
} UnkStruct_022d15f4;

void func_ov000_022d4654(s32 a);

void Boss2_State_Unk_ov000_022d15f4(UnkStruct_022d15f4 *s) {
    if (!(s->unkBA8 & 2))
        return;
    func_ov000_022d4654(2);
}
