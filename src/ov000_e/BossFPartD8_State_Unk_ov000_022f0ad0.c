#include <nitro/types.h>

typedef struct UnkStruct_022f0ad0 UnkStruct_022f0ad0;
struct UnkStruct_022f0ad0 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1[0x68 - 0x1c];
    void (*func)(UnkStruct_022f0ad0 *s);
    u8 pad2[0x440 - 0x6c];
    s32 unk440;
    s32 unk444;
    s32 unk448;
    u16 unk44C;
};

typedef struct UnkStruct_022f0ad0_c {
    u8 pad[0x78];
    s32 unk78;
    u8 pad1[0x84 - 0x7c];
    s32 unk84;
} UnkStruct_022f0ad0_c;

extern UnkStruct_022f0ad0_c data_ov000_0236a48c;

void func_ov000_022f4ce4(UnkStruct_022f0ad0 *s, s32 a, s32 b, s32 c);
void BossFPartD8_State_Unk_ov000_022f09fc(UnkStruct_022f0ad0 *s);

void BossFPartD8_State_Unk_ov000_022f0ad0(UnkStruct_022f0ad0 *s) {
    UnkStruct_022f0ad0_c *c;
    func_ov000_022f4ce4(s, 0x20, 0x1000, 0);
    s->unk440 = s->unk10 << 4;
    s->unk444 = s->unk14 << 4;
    s->unk448 = s->unk18 << 4;
    s->unk44C = 0;
    c = (UnkStruct_022f0ad0_c *)(u32)&data_ov000_0236a48c;
    c->unk78 = 0x258000;
    c->unk84 = 0x14000;
    s->func = BossFPartD8_State_Unk_ov000_022f09fc;
}
