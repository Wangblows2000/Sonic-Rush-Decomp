#include <nitro/types.h>

typedef struct { u8 unk0[0x84]; u32 unk84; } UnkStruct_0201d0b8;

void SymbolDisposeCallback(void *a, void *b, UnkStruct_0201d0b8 *s) {
    s->unk84 = 0;
}
