#include <nitro/types.h>

typedef struct UnkStruct_022f66d0 {
    u8 pad[0x14];
    s32 unk14;
    u8 pad2[0x68 - 0x18];
    void (*func)(void);
} UnkStruct_022f66d0;

void BossFPartDE_State_Land(void);

void BossFPartDE_State_Fall(UnkStruct_022f66d0 *s) {
    if (s->unk14 > 0x24c00)
        s->func = BossFPartDE_State_Land;
}
