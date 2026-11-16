#include <nitro/types.h>

typedef struct UnkStruct_02307a90 {
    u8 pad[0x13e];
    u16 unk13E;
} UnkStruct_02307a90;

void DashP_State_Unk_ov000_02307a90(UnkStruct_02307a90 *s) {
    s->unk13E &= ~0x100;
}
