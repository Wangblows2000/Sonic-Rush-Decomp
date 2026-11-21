#include <nitro/types.h>

typedef struct UnkStruct_023099b0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_023099b0;

void MapObj8C_State_WaitPlayerEnter(void);

void MapObj8C_Action_Init(UnkStruct_023099b0 *s) {
    s->func = MapObj8C_State_WaitPlayerEnter;
}
