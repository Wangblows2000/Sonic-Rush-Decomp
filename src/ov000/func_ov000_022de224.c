#include <nitro/types.h>

typedef struct UnkStruct_022de224 {
    u8 pad[0x220];
    void (*func)(void);
    s32 unk224;
    u8 pad1[0xbc4 - 0x228];
    s32 unkBC4;
    s32 unkBC8;
    s32 unkBCC;
} UnkStruct_022de224;

extern s32 data_ov000_02369ac0[];

void func_ov000_022de0d0(void);
void func_ov000_022db900(UnkStruct_022de224 *s, s32 a);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);

void func_ov000_022de224(UnkStruct_022de224 *s) {
    data_ov000_02369ac0[1] = 0x15;
    s->func = func_ov000_022de0d0;
    s->unk224 = 0;
    func_ov000_022db900(s, 0x20);
    s->unkBC4 = 0x180;
    s->unkBC8 = 0xd80;
    s->unkBCC = 0x900;
    Sound_PlaySeqArc(0, 0, 0x8d);
}
