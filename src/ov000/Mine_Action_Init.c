#include <nitro/types.h>

typedef struct UnkStruct_0231c99c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231c99c;

void Mine_State_Main(void);

void Mine_Action_Init(UnkStruct_0231c99c *s) {
    s->func = Mine_State_Main;
}
