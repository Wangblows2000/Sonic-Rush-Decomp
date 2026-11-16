#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    void *(*alloc)(void *heap, u32 size, void *ptr);
    void *heap;
    u8 pad1c[0xc];
    s32 unk28;
} UnkCtx_0203cb9c;

typedef struct {
    u8 pad0[0x80];
    void *buf;
    u8 pad84[0xaf8 - 0x84];
    void *work;
    u8 padafc[4];
    u32 size;
} UnkObj_0203cb9c;

extern UnkCtx_0203cb9c data_022b8be0;
u32 OS_DisableInterrupts(void);
u32 OS_RestoreInterrupts(u32 e);

void WFSi_ReallocBitmap(UnkObj_0203cb9c *o, s32 size) {
    void *w;
    u32 n;
    u32 e;
    if (size < 0) {
        size = o->size;
        o->size = 0;
    }
    if (o->size >= (u32)size)
        return;
    o->size = size;
    w = o->work;
    if (w != NULL) {
        e = OS_DisableInterrupts();
        data_022b8be0.alloc(data_022b8be0.heap, 0, w);
        OS_RestoreInterrupts(e);
    }
    {
        u32 d = data_022b8be0.unk28 - 0xe;
        n = ((o->size + d - 1) / d + 0x1f) / 4 * 16;
    }
    e = OS_DisableInterrupts();
    w = data_022b8be0.alloc(data_022b8be0.heap, n, NULL);
    OS_RestoreInterrupts(e);
    o->work = w;
    o->buf = o->work;
}
