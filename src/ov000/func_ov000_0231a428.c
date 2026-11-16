#include <nitro/types.h>

typedef struct UnkStruct_0231a428 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231a428;

void StageObj_SetAnimation(UnkStruct_0231a428 *s, s32 a);
void func_ov000_0231a3fc(void);

void func_ov000_0231a428(UnkStruct_0231a428 *s) {
    StageObj_SetAnimation(s, 1);
    s->func = func_ov000_0231a3fc;
}
