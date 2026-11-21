#include <nitro/types.h>

typedef struct UnkStruct_0232957c_2 {
    u8 pad[0x62];
    u16 unk62;
} UnkStruct_0232957c_2;

typedef struct UnkStruct_0232957c {
    u8 pad[0x14];
    s32 unk14;
    u8 pad1[0x2b0 - 0x18];
    s32 unk2B0;
} UnkStruct_0232957c;

extern UnkStruct_0232957c_2 gCamera;

void BossEffect_State_Main(UnkStruct_0232957c *s);

void EffSplash3D_State_Main(UnkStruct_0232957c *s) {
    s->unk14 = (gCamera.unk62 << 8) + (s->unk2B0 << 8);
    BossEffect_State_Main(s);
}
