#include <nitro/types.h>

BOOL NNS_SndStrmAllocChannel(void *a, void *b, void *c);

typedef struct { u8 unk0[0x11C]; u32 unk11C; } UnkStruct_0201fb10;

BOOL func_0201fb10(UnkStruct_0201fb10 *s, void *b, void *c) {
    if (s->unk11C == 0) {
        if (!NNS_SndStrmAllocChannel(s, b, c)) return FALSE;
    }
    s->unk11C++;
    return TRUE;
}
