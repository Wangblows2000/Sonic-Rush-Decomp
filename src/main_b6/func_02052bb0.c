#include <nitro/types.h>

typedef struct {
    u16 count;
    u16 flags;
    u8 *loop;
} UnkSeq_02052bb0;

typedef struct {
    u8 pad0[8];
    void *dst;
    void *file;
    u8 pad10[0xc4];
    u32 flags;
    u8 padd8[0x10];
    s32 idx;
    s32 timer;
    UnkSeq_02052bb0 *seq;
    u8 *cur;
} UnkWork_02052bb0;

extern u8 *data_022c466c;

void func_020525d8(void *f, void *dst);

void func_02052bb0(UnkWork_02052bb0 *w) {
    if (w->flags & 0x100)
        return;
    w->timer++;
    if (*w->cur > w->timer)
        return;
    w->timer = 0;
    w->idx++;
    if (w->idx >= w->seq->count) {
        if (w->seq->flags & 1) {
            w->idx = 0;
            w->cur = w->seq->loop;
        } else {
            w->flags |= 0x100;
            return;
        }
    } else {
        w->cur += 2;
    }
    w->file = data_022c466c + w->cur[1] * 0x44;
    func_020525d8(w->file, w->dst);
    w->flags |= 0x40000;
}
