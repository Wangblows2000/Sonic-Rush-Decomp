#include <nitro/types.h>

typedef struct UnkStruct_022f1aac {
    u8 pad[0x68];
    void (*func)(void);
} UnkStruct_022f1aac;

void func_ov000_022f9188(UnkStruct_022f1aac *s);
void BossFPartD8_State_Unk_ov000_022f19a0(void);

void BossFPartD8_Action_Unk_ov000_022f1aac(UnkStruct_022f1aac *s) {
    func_ov000_022f9188(s);
    s->func = BossFPartD8_State_Unk_ov000_022f19a0;
}
