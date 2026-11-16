#include <nitro/types.h>

typedef struct UnkStruct_022f3dac {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x510];
    u8 unk57C;
} UnkStruct_022f3dac;

void BossFPartD8_Unk_ov000_022f49f4(UnkStruct_022f3dac *s);
void BossFPartD8_State_Unk_ov000_022f3d48(void);

void BossFPartD8_State_Unk_ov000_022f3dac(UnkStruct_022f3dac *s) {
    BossFPartD8_Unk_ov000_022f49f4(s);
    if (s->unk57C & 2)
        s->func = BossFPartD8_State_Unk_ov000_022f3d48;
}
