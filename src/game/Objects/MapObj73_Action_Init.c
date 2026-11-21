#include <nitro/types.h>

typedef struct UnkStruct_02303e24 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02303e24;

void MapObj73_State_Main(void);

void MapObj73_Action_Init(UnkStruct_02303e24 *s) {
    s->func = MapObj73_State_Main;
}
