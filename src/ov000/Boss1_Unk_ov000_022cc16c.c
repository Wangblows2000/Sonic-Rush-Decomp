#include <nitro/types.h>

typedef struct UnkStruct_022cc114_2 {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022cc114_2;

typedef struct UnkStruct_022cc16c {
    u8 pad[0x210];
    UnkStruct_022cc114_2 *unk210;
} UnkStruct_022cc16c;

extern UnkStruct_022cc16c *gBoss1;

void Boss1_State_Unk_ov000_022c895c(void);

BOOL Boss1_Unk_ov000_022cc16c(void) {
    return Boss1_State_Unk_ov000_022c895c == gBoss1->unk210->func;
}
