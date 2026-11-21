#include <nitro/types.h>

typedef struct { u8 unk0[0x80]; u32 unk80; } UnkStruct_0201d0c4;

void FatDisposeCallback(void *a, void *b, UnkStruct_0201d0c4 *s) {
    s->unk80 = 0;
}
