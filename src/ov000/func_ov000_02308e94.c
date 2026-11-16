#include <nitro/types.h>

typedef struct UnkStruct_02308e94 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02308e94;

void StageObj_SetAnimation(UnkStruct_02308e94 *s, s32 a);
void func_ov000_02308df8(void);

void func_ov000_02308e94(UnkStruct_02308e94 *s) {
    StageObj_SetAnimation(s, 0);
    s->func = func_ov000_02308df8;
}
