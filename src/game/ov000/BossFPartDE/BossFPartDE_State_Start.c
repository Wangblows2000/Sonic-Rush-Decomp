#include <nitro/types.h>

typedef struct UnkStruct_022f6834 UnkStruct_022f6834;
struct UnkStruct_022f6834 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022f6834 *s);
};

void BossFPartDE_State_StartRise(UnkStruct_022f6834 *s);

void BossFPartDE_State_Start(UnkStruct_022f6834 *s) {
    s->func = BossFPartDE_State_StartRise;
    s->func(s);
}
