#include <nitro/types.h>

typedef struct UnkStruct_02313310 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02313310;

void GmkRail_State_Move(void);

void GmkRail_Action_Init(UnkStruct_02313310 *s) {
    s->func = GmkRail_State_Move;
}
