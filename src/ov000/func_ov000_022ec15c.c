#include <nitro/types.h>

typedef struct UnkStruct_022ec15c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022ec15c;

void Boss7_State_Unk_ov000_022ebf44(void);

void func_ov000_022ec15c(UnkStruct_022ec15c *s) {
    s->func = Boss7_State_Unk_ov000_022ebf44;
}
