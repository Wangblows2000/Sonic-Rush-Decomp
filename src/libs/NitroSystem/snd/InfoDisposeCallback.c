#include <nitro/types.h>

typedef struct { u8 unk0[0x88]; u32 unk88; } UnkStruct_0201d0d0;

void InfoDisposeCallback(void *a, void *b, UnkStruct_0201d0d0 *s) {
    s->unk88 = 0;
}
