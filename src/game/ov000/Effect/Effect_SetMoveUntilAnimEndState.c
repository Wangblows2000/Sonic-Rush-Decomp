#include <nitro/types.h>

typedef struct UnkStruct_02334264 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02334264;

void Effect_State_MoveUntilAnimEnd(void);

void Effect_SetMoveUntilAnimEndState(UnkStruct_02334264 *s) {
    s->func = Effect_State_MoveUntilAnimEnd;
}
