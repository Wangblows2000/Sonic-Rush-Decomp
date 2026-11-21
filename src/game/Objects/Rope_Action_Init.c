#include <nitro/types.h>

typedef struct UnkStruct_0231478c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231478c;

void Rope_State_Attached(void);

void Rope_Action_Init(UnkStruct_0231478c *s) {
    s->func = Rope_State_Attached;
}
