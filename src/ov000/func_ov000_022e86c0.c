#include <nitro/types.h>

typedef struct UnkStruct_022e86c0_2 {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022e86c0_2;

typedef struct UnkStruct_022e86c0 UnkStruct_022e86c0;
struct UnkStruct_022e86c0 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad1[0x5c - 0x14];
    u16 unk5C;
    u8 pad2[0x440 - 0x5e];
    void (*func)(UnkStruct_022e86c0 *s);
};

extern UnkStruct_022e86c0_2 *gPlayers;

void func_ov000_022e9b70(UnkStruct_022e86c0 *s, s32 a, s32 b);
void func_ov000_022e8644(UnkStruct_022e86c0 *s);

void func_ov000_022e86c0(UnkStruct_022e86c0 *s) {
    func_ov000_022e9b70(s, 1, 0);
    if (s->unk10 > gPlayers->unk10)
        s->unk5C |= 1;
    else
        s->unk5C &= ~1;
    s->func = func_ov000_022e8644;
    s->func(s);
}
