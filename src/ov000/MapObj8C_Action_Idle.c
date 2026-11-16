#include <nitro/types.h>

typedef struct UnkStruct_023093e8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_023093e8;

void MapObj8C_State_Idle(void);

void MapObj8C_Action_Idle(UnkStruct_023093e8 *s) {
    s->func = MapObj8C_State_Idle;
}
