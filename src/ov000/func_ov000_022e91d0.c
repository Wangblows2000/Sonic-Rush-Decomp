#include <nitro/types.h>

typedef struct UnkStruct_022e91d0 UnkStruct_022e91d0;
struct UnkStruct_022e91d0 {
    u8 pad[0x2e];
    u16 unk2E;
    u8 pad1[0x58 - 0x30];
    u32 unk58;
    u8 pad2[0x440 - 0x5c];
    void (*func)(UnkStruct_022e91d0 *s);
};

extern s32 data_ov000_02369ac0[];

void func_ov000_022ee198(UnkStruct_022e91d0 *s, s32 a);
void func_ov000_022e9b70(UnkStruct_022e91d0 *s, s32 a, s32 b);
void func_ov000_022e9170(UnkStruct_022e91d0 *s);

void func_ov000_022e91d0(UnkStruct_022e91d0 *s) {
    s->unk58 |= 0x10;
    s->unk2E = 0;
    func_ov000_022ee198(s, 3);
    func_ov000_022e9b70(s, 1, 0);
    data_ov000_02369ac0[1] = 0x27;
    s->func = func_ov000_022e9170;
    s->func(s);
}
