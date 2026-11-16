#include <nitro/types.h>

typedef struct UnkStruct_0231a450 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_0231a450;

void StageObj_SetAnimation(UnkStruct_0231a450 *s, s32 a);
void func_ov000_0231a44c(void);

void func_ov000_0231a450(UnkStruct_0231a450 *s) {
    StageObj_SetAnimation(s, 0);
    s->func = func_ov000_0231a44c;
}
