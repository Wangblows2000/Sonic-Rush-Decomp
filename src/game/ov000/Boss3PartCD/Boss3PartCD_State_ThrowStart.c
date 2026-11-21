#include <nitro/types.h>

typedef struct UnkStruct_022d59e0 {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad2[0x20c - 0x6c];
    u32 unk20C;
} UnkStruct_022d59e0;

void Boss3PartCD_State_Harmful(void);

void Boss3PartCD_State_ThrowStart(UnkStruct_022d59e0 *s) {
    s->unk20C |= 4;
    s->func = Boss3PartCD_State_Harmful;
}
