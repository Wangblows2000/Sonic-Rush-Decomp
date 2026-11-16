#include <nitro/types.h>

typedef struct UnkStruct_0231ac10 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x68 - 0x58];
    void (*func)(void);
} UnkStruct_0231ac10;

void Valve_SetSpriteAnimEnd(UnkStruct_0231ac10 *s);
void Valve_State_End(void);

void Valve_Action_End(UnkStruct_0231ac10 *s) {
    s->unk54 |= 2;
    Valve_SetSpriteAnimEnd(s);
    s->func = Valve_State_End;
}
