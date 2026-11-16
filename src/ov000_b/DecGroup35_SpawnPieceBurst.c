#include <nitro/types.h>

extern u32 gRandSeed;

void DecGroup35Piece_Create(s32 x, s32 y, s16 a, s16 b, s32 c);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void DecGroup35_SpawnPieceBurst(s32 x, s32 y) {
    DecGroup35Piece_Create(x + (0xfff - (Rand() & 0x1ffe)), y + (0x7ff - (Rand() & 0xffe)), 0xff - (Rand() & 0x1fe), 0xff - (Rand() & 0x1fe), 0);
    DecGroup35Piece_Create(x + (0xfff - (Rand() & 0x1ffe)), y + (0x7ff - (Rand() & 0xffe)), 0xff - (Rand() & 0x1fe), -1 - (Rand() & 0x1fe), 1);
    DecGroup35Piece_Create(x + (0xfff - (Rand() & 0x1ffe)), y + (0x7ff - (Rand() & 0xffe)), 0x7f - (Rand() & 0x1fe), 0xff - (Rand() & 0x1fe), 2);
    DecGroup35Piece_Create(x + (0xfff - (Rand() & 0x1ffe)), y + (0x7ff - (Rand() & 0xffe)), 0x17f - (Rand() & 0x1fe), -0x81 - (Rand() & 0x1fe), 3);
}
