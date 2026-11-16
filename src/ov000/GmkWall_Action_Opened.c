#include <nitro/types.h>

typedef struct UnkStruct_02308a6c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02308a6c;

void GmkWall_State_Opened(void);

void GmkWall_Action_Opened(UnkStruct_02308a6c *s) {
    s->func = GmkWall_State_Opened;
}
