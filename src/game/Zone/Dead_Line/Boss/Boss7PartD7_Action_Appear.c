#include <nitro/types.h>

typedef struct UnkStruct_022ec15c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022ec15c;

void Boss7PartD7_State_StartAppear(void);

void Boss7PartD7_Action_Appear(UnkStruct_022ec15c *s) {
    s->func = Boss7PartD7_State_StartAppear;
}
