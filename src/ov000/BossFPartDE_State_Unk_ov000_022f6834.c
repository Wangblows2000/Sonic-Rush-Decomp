#include <nitro/types.h>

typedef struct UnkStruct_022f6834 UnkStruct_022f6834;
struct UnkStruct_022f6834 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022f6834 *s);
};

void BossFPartDE_State_Unk_ov000_022f67c8(UnkStruct_022f6834 *s);

void BossFPartDE_State_Unk_ov000_022f6834(UnkStruct_022f6834 *s) {
    s->func = BossFPartDE_State_Unk_ov000_022f67c8;
    s->func(s);
}
