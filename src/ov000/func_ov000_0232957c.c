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

extern UnkStruct_0232957c_2 data_ov000_0236aad8;

void func_ov000_02327f24(UnkStruct_0232957c *s);

void func_ov000_0232957c(UnkStruct_0232957c *s) {
    s->unk14 = (data_ov000_0236aad8.unk62 << 8) + (s->unk2B0 << 8);
    func_ov000_02327f24(s);
}
