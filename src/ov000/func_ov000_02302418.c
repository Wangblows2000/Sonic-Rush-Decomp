#include <nitro/types.h>

typedef struct UnkStruct_02302418 {
    u8 pad[0x30];
    s16 unk30;
    u8 pad1[0x44 - 0x32];
    u16 unk44;
    u16 unk46;
    u8 pad2[0x54 - 0x48];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad3[0x68 - 0x5e];
    void (*func)(void);
} UnkStruct_02302418;

extern u32 data_0208fe70;

void func_02069818(UnkStruct_02302418 *s, s32 a);
void func_ov000_02302398(void);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov000_02302418(UnkStruct_02302418 *s) {
    func_02069818(s, 0);
    s->unk5C |= 4;
    s->unk58 |= 0x10;
    s->unk44 = 0;
    s->unk46 = 0;
    s->unk30 = (-0xc1 - (Rand() & 0x1e)) >> 1;
    s->unk54 |= 2;
    s->func = func_ov000_02302398;
}
