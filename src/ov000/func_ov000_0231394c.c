#include <nitro/types.h>

typedef struct UnkStruct_0231394c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231394c;

void StageObj_SetAnimation(UnkStruct_0231394c *s, s32 a);
void func_ov000_02313948(void);

void func_ov000_0231394c(UnkStruct_0231394c *s) {
    StageObj_SetAnimation(s, 0);
    s->func = func_ov000_02313948;
}
