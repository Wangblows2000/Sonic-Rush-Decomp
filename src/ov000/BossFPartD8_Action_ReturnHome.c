#include <nitro/types.h>

typedef struct UnkStruct_022f1aac {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022f1aac;

void BossF_InitCamera(UnkStruct_022f1aac *s);
void BossFPartD8_State_ReturnHome(void);

void BossFPartD8_Action_ReturnHome(UnkStruct_022f1aac *s) {
    BossF_InitCamera(s);
    s->func = BossFPartD8_State_ReturnHome;
}
