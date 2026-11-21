#include <nitro/types.h>

typedef struct UnkStruct_022f58ac {
    u8 pad[0x14];
    s32 unk14;
    u8 pad1[0x50];
    void (*func)(void);
} UnkStruct_022f58ac;

void BossFPartDE_State_ChainLand(void);

void BossFPartDE_State_ChainFall(UnkStruct_022f58ac *s) {
    if (s->unk14 > 0x25600)
        s->func = BossFPartDE_State_ChainLand;
}
