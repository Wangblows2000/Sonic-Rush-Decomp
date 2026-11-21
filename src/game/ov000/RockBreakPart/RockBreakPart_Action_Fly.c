#include <nitro/types.h>

typedef struct UnkStruct_023136dc {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_023136dc;

void RockBreakPart_State_Fly(void);

void RockBreakPart_Action_Fly(UnkStruct_023136dc *s) {
    s->func = RockBreakPart_State_Fly;
}
