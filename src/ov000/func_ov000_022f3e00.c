#include <nitro/types.h>

typedef struct UnkStruct_022f3e00 {
    u8 pad[0x2e];
    u16 unk2E;
    u8 pad1[0x68 - 0x30];
    void (*func)(void);
    u8 pad2[0x434 - 0x6c];
    s32 unk434;
    u16 unk438;
    u16 unk43A;
} UnkStruct_022f3e00;

extern u32 data_0208fe70;
extern s32 data_ov000_02369ac0[];

void func_ov000_022f9044(UnkStruct_022f3e00 *s);
void func_ov000_022f4cbc(UnkStruct_022f3e00 *s);
void func_020366f4(s32 a, s32 b, s32 c);
void func_ov000_022f3dd4(void);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov000_022f3e00(UnkStruct_022f3e00 *s) {
    s->unk2E = 0;
    func_ov000_022f9044(s);
    func_ov000_022f4cbc(s);
    s->unk434 = (Rand() & 1) ? 1 : 0;
    s->unk438 = 0;
    s->unk43A = 0;
    func_020366f4(0, 0, 0xd0);
    data_ov000_02369ac0[1] = 0x30;
    s->func = func_ov000_022f3dd4;
}
