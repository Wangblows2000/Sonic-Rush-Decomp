#include <nitro/types.h>

typedef struct UnkStruct_022d68a4_a {
    u8 pad[0x10c];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d68a4_a;

typedef struct UnkStruct_022d68a4 UnkStruct_022d68a4;
struct UnkStruct_022d68a4 {
    u8 pad[0x14];
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    u16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d68a4 *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d68a4_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

void func_ov000_022dac94(u16 *p, s32 a, s32 b);
BOOL func_ov000_022d8c64(UnkStruct_022d68a4 *s);
void Boss3PartCC_State_Unk_ov000_022d66cc(UnkStruct_022d68a4 *s);

void Boss3PartCC_State_Unk_ov000_022d68a4(UnkStruct_022d68a4 *s) {
    func_ov000_022dac94(&s->unk264, 1, 5);
    if (func_ov000_022d8c64(s)) {
        s->unk264 = 0;
        s->func = Boss3PartCC_State_Unk_ov000_022d66cc;
        return;
    }
}
