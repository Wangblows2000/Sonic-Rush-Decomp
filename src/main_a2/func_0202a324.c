#include <nitro/types.h>

typedef struct { char *buf; u32 size; u32 pos; } UnkCtx_0202a324;

void *func_0202a398(void *ctx, const char *s, u32 n);
u32 func_0202a3e8(void *(*write)(void *, const char *, u32), void *ctx, const char *fmt, void *args);

u32 func_0202a324(char *buf, u32 size, const char *fmt, void *args) {
    UnkCtx_0202a324 ctx;
    u32 n;
    ctx.buf = buf;
    ctx.size = size;
    ctx.pos = 0;
    n = func_0202a3e8(func_0202a398, &ctx, fmt, args);
    if (buf != NULL) {
        if (n < size) {
            buf[n] = 0;
        } else if (size != 0) {
            buf[size - 1] = 0;
        }
    }
    return n;
}
