#include <nitro/types.h>

typedef struct UnkStruct_0231797c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231797c;

void Swing_State_Main(void);

void Swing_Action_Init(UnkStruct_0231797c *s) {
    s->func = Swing_State_Main;
}
