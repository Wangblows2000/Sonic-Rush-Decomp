#include <nitro/types.h>

typedef struct UnkStruct_022f24f8 {
    u8 pad[0x57c];
    u8 unk57C;
} UnkStruct_022f24f8;

void func_ov000_022f88a4(void);

void BossFPartD8_State_Unk_ov000_022f24f8(UnkStruct_022f24f8 *s) {
    if (!(s->unk57C & 2))
        return;
    func_ov000_022f88a4();
}
