#include <nitro/types.h>

typedef struct UnkStruct_0231a3fc {
    u8 pad[0x5c];
    u16 unk5C;
} UnkStruct_0231a3fc;

void Trampoline_Action_Idle(UnkStruct_0231a3fc *s);

void Trampoline_State_Bounce(UnkStruct_0231a3fc *s) {
    if (!(s->unk5C & 8))
        return;
    Trampoline_Action_Idle(s);
}
