#include <nitro/types.h>

typedef struct UnkStruct_0231d594 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231d594;

void func_ov000_0231d3b8(void);

void Wheel2D_Action_Init(UnkStruct_0231d594 *s) {
    s->func = func_ov000_0231d3b8;
}
