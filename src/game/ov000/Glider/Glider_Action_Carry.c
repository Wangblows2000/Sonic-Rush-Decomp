#include <nitro/types.h>

typedef struct UnkStruct_0230ca40 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0230ca40;

void Glider_State_Carry(void);

void Glider_Action_Carry(UnkStruct_0230ca40 *s) {
    s->func = Glider_State_Carry;
}
