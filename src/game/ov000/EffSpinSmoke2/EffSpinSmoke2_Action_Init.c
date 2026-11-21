#include <nitro/types.h>

typedef struct UnkStruct_02333bd0 {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad2[0x68 - 0x5e];
    void (*func)(void);
} UnkStruct_02333bd0;

void EffSpinSmoke2_State_Main(void);

void EffSpinSmoke2_Action_Init(UnkStruct_02333bd0 *s) {
    s->unk5C |= 4;
    s->func = EffSpinSmoke2_State_Main;
}
