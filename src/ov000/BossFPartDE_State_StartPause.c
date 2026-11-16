#include <nitro/types.h>

typedef struct UnkStruct_022f62bc UnkStruct_022f62bc;
struct UnkStruct_022f62bc {
    u8 pad[0x68];
    void (*func)(UnkStruct_022f62bc *s);
    u8 pad1[0x4ae - 0x6c];
    u16 unk4AE;
};

void BossFPartDE_State_Pause(UnkStruct_022f62bc *s);

void BossFPartDE_State_StartPause(UnkStruct_022f62bc *s) {
    s->unk4AE = 0;
    s->func = BossFPartDE_State_Pause;
    s->func(s);
}
