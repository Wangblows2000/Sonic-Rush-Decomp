#include <nitro/types.h>

void *func_0202a224(void *dst, const void *src, u32 n);

typedef struct { u8 *buf; u32 size; u32 pos; } UnkStruct_0202a398;

BOOL func_0202a398(UnkStruct_0202a398 *s, const void *src, u32 n) {
    if (s->pos + n > s->size) n = s->size - s->pos;
    func_0202a224(s->buf + s->pos, src, n);
    s->pos += n;
    return TRUE;
}
