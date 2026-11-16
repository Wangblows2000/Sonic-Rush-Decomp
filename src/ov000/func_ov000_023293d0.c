#include <nitro/types.h>

typedef struct UnkStruct_023293d0_2 {
    u8 pad[0x62];
    u16 unk62;
} UnkStruct_023293d0_2;

typedef struct UnkStruct_023293d0 {
    u8 pad[0x14];
    s32 unk14;
    u8 pad1[0x2b0 - 0x18];
    s32 unk2B0;
} UnkStruct_023293d0;

extern UnkStruct_023293d0_2 gCamera;

void func_ov000_02327f24(UnkStruct_023293d0 *s);

void func_ov000_023293d0(UnkStruct_023293d0 *s) {
    s->unk14 = s->unk2B0 + (gCamera.unk62 << 8);
    func_ov000_02327f24(s);
}
