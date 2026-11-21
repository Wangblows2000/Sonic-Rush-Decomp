#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    s32 unkC;
    u8 pad10[0x1f4 - 0x10];
    u32 unk1F4;
} UnkStruct_02304f50;

void Cargo_Action_StartMove(UnkStruct_02304f50 *p, s32 a);

void Cargo_State_Stopped(UnkStruct_02304f50 *p) {
    if (!(p->unk1F4 & 4))
        return;
    p->unkC++;
    if (p->unkC <= 0x78)
        return;
    Cargo_Action_StartMove(p, 0);
}
