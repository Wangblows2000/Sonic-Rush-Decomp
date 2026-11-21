#include <nitro/types.h>

typedef struct UnkStruct_023122d8 {
    u8 pad[0xc];
    s32 unkC;
    u8 pad1[0x54 - 0x10];
    u32 unk54;
} UnkStruct_023122d8;

void GmkPipe_State_Cooldown(UnkStruct_023122d8 *s) {
    if (s->unkC == 0)
        return;
    s->unkC--;
    if (s->unkC == 0)
        s->unk54 &= ~2;
}
