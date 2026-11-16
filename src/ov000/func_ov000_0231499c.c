#include <nitro/types.h>

typedef struct UnkStruct_0231499c {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231499c;

void StageObj_SetAnimation(UnkStruct_0231499c *s, s32 a);
void func_ov000_0231492c(void);

void func_ov000_0231499c(UnkStruct_0231499c *s) {
    StageObj_SetAnimation(s, 0);
    s->func = func_ov000_0231492c;
}
