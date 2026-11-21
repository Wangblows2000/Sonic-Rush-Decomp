#include <nitro/types.h>

typedef struct UnkStruct_0231d594 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231d594;

void Wheel2D_State_Main(void);

void Wheel2D_Action_Init(UnkStruct_0231d594 *s) {
    s->func = Wheel2D_State_Main;
}
