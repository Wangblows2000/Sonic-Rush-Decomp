#include <nitro/types.h>

typedef struct UnkStruct_02314bb8 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_02314bb8;

void StageObj_SetAnimation(UnkStruct_02314bb8 *s, s32 a);
void func_ov000_02314b78(void);

void func_ov000_02314bb8(UnkStruct_02314bb8 *s) {
    StageObj_SetAnimation(s, 0);
    s->func = func_ov000_02314b78;
}
