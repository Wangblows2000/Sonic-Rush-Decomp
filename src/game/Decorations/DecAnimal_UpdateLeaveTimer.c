#include <nitro/types.h>

typedef struct UnkStruct_0231db90 {
    u8 pad[0x8];
    s32 unk8;
} UnkStruct_0231db90;

void DecAnimal_Action_Leave(UnkStruct_0231db90 *s);

void DecAnimal_UpdateLeaveTimer(UnkStruct_0231db90 *s) {
    if (s->unk8 != 0) {
        s->unk8--;
        return;
    }
    DecAnimal_Action_Leave(s);
}
