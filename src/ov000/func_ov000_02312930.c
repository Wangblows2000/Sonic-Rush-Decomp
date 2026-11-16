#include <nitro/types.h>

typedef struct UnkStruct_02312930 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02312930;

void StageObj_SetAnimation(UnkStruct_02312930 *s, s32 a);
void func_ov000_023128c4(void);

void func_ov000_02312930(UnkStruct_02312930 *s) {
    StageObj_SetAnimation(s, 0);
    s->func = func_ov000_023128c4;
}
