#include <nitro/types.h>

typedef struct UnkStruct_022c6900_2 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022c6900_2;

typedef struct UnkStruct_022c6900 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022c6900;

extern UnkStruct_022c6900_2 *gBoss1;

void func_ov000_022c68ac(void);

void Boss1Hammer_State_Unk_ov000_022c6900(UnkStruct_022c6900 *s) {
    if (gBoss1->unk20C & 1)
        s->func = func_ov000_022c68ac;
}
