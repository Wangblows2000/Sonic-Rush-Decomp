#include <nitro/types.h>

void *memcpy(void *dst, const void *src, u32 n);

typedef struct { u8 *buf; u32 size; u32 pos; } UnkStruct_0202a398;

BOOL __StringWrite(UnkStruct_0202a398 *s, const void *src, u32 n) {
    if (s->pos + n > s->size) n = s->size - s->pos;
    memcpy(s->buf + s->pos, src, n);
    s->pos += n;
    return TRUE;
}
