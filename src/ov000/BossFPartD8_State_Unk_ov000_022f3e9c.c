#include <nitro/types.h>

typedef struct UnkStruct_022f3e9c {
    u8 pad[0x434];
    u16 unk434;
} UnkStruct_022f3e9c;

void BossFPartD8_Unk_ov000_022f4a3c(UnkStruct_022f3e9c *s);
void BossFPartD8_Unk_ov000_022f419c(void);

void BossFPartD8_State_Unk_ov000_022f3e9c(UnkStruct_022f3e9c *s) {
    BossFPartD8_Unk_ov000_022f4a3c(s);
    s->unk434--;
    if (s->unk434 != 0)
        return;
    BossFPartD8_Unk_ov000_022f419c();
}
