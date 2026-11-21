#include <nitro/types.h>

typedef struct UnkStruct_022d9aa0 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022d9aa0;

void Boss3_State_DefeatCamera1(void);

void Boss3_State_StartDefeat(UnkStruct_022d9aa0 *s) {
    s->func = Boss3_State_DefeatCamera1;
}
