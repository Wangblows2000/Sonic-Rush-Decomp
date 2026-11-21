#include <nitro/types.h>

extern u32 gRandSeed;
void EffTrickComboPart_Create(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void EffTrickCombo_Spawn(void) {
    switch ((u16)(Rand() & 7)) {
    case 0:
        EffTrickComboPart_Create(0, -0x30, 0x30, 0, -0x800, 0);
        EffTrickComboPart_Create(1, 0, 0, 0, -0x800, 0);
        EffTrickComboPart_Create(2, 0x30, 0x30, 0, -0x800, 0);
        break;
    case 1:
        EffTrickComboPart_Create(0, 0x60, 0x18, 0, -0x700, 0);
        EffTrickComboPart_Create(1, 0x40, 0x18, 0, -0x700, 4);
        EffTrickComboPart_Create(2, 0x20, 0x18, 0, -0x700, 8);
        EffTrickComboPart_Create(0, 0, 0x18, 0, -0x700, 0xc);
        EffTrickComboPart_Create(1, -0x20, 0x18, 0, -0x700, 0x10);
        EffTrickComboPart_Create(2, -0x40, 0x18, 0, -0x700, 0x14);
        EffTrickComboPart_Create(0, -0x60, 0x18, 0, -0x700, 0x18);
        break;
    case 2:
        EffTrickComboPart_Create(0, -0x50, 0x18, 0, -0x780, 0);
        EffTrickComboPart_Create(1, -0x20, 0x18, 0, -0x780, 8);
        EffTrickComboPart_Create(2, 0x20, 0x18, 0, -0x780, 8);
        EffTrickComboPart_Create(0, 0x50, 0x18, 0, -0x780, 0);
        break;
    case 3:
        EffTrickComboPart_Create(0, 0x40, 0x18, 0, -0x780, 4);
        EffTrickComboPart_Create(2, 0x20, 0x18, 0, -0x780, 0xc);
        EffTrickComboPart_Create(1, 0, 0x18, 0, -0x800, 0x18);
        EffTrickComboPart_Create(2, -0x20, 0x18, 0, -0x780, 0x10);
        EffTrickComboPart_Create(0, -0x40, 0x18, 0, -0x780, 8);
        break;
    case 4:
        EffTrickComboPart_Create(2, -0x50, 0x18, 0x180, -0x780, 0);
        EffTrickComboPart_Create(1, 0, 0x18, 0, -0x800, 0x10);
        EffTrickComboPart_Create(2, 0x50, 0x18, -0x180, -0x780, 0);
        break;
    case 5:
        EffTrickComboPart_Create(0, 0x64, 0x18, 0, -0x700, 0);
        EffTrickComboPart_Create(1, 0x3c, 0x18, 0, -0x780, 8);
        EffTrickComboPart_Create(2, 0x14, 0x18, 0, -0x800, 0x10);
        EffTrickComboPart_Create(1, -0x14, 0x18, 0, -0x700, 0);
        EffTrickComboPart_Create(2, -0x3c, 0x18, 0, -0x780, 8);
        EffTrickComboPart_Create(0, -0x64, 0x18, 0, -0x800, 0x10);
        break;
    case 6:
        EffTrickComboPart_Create(0, -0x6c, 0x18, 0x500, -0x700, 0);
        EffTrickComboPart_Create(2, -0x30, 0x18, 0x200, -0x800, 8);
        EffTrickComboPart_Create(2, 0x30, 0x18, -0x200, -0x800, 8);
        EffTrickComboPart_Create(0, 0x6c, 0x18, -0x500, -0x700, 0);
        break;
    case 7:
        EffTrickComboPart_Create(1, 0x40, 0x18, 0, -0x780, 0x18);
        EffTrickComboPart_Create(0, 0x20, 0x18, 0, -0x780, 0xc);
        EffTrickComboPart_Create(2, 0, 0x18, 0, -0x800, 0);
        EffTrickComboPart_Create(0, -0x20, 0x18, 0, -0x780, 0xc);
        EffTrickComboPart_Create(1, -0x40, 0x18, 0, -0x780, 0x18);
        break;
    }
}
