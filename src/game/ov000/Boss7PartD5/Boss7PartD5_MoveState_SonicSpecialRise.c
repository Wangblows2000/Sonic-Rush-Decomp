#include <nitro/types.h>

typedef struct UnkStruct_022e8454 {
    u8 pad[0x2e];
    u16 unk2E;
    s16 unk30;
    u8 pad1[0x40e];
    void (*func)(void);
} UnkStruct_022e8454;

void Boss7PartD5_MoveState_SonicSpecialStartOrbit(void);

void Boss7PartD5_MoveState_SonicSpecialRise(UnkStruct_022e8454 *s) {
    if (s->unk30 > -0x100) {
        s->unk2E = 0;
        s->func = Boss7PartD5_MoveState_SonicSpecialStartOrbit;
    }
}
