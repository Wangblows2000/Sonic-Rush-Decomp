#include <nitro/types.h>

typedef struct { u8 unk0[0xA0]; u32 unkA0; } UnkStruct_0202205c;
extern UnkStruct_0202205c data_02084d38;

void func_0202205c(u32 *a, u32 *b, u32 *c, u32 *d) {
    if (a != NULL) *a = data_02084d38.unkA0 & 0xFF;
    if (b != NULL) *b = (data_02084d38.unkA0 >> 8) & 0xFF;
    if (c != NULL) *c = (data_02084d38.unkA0 >> 16) & 0xFF;
    if (d != NULL) *d = (data_02084d38.unkA0 >> 24) & 0xFF;
}
