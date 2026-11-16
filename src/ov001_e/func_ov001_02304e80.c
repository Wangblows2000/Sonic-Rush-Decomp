#include <nitro/types.h>

typedef struct UnkStruct_02304e80_f {
    u16 idx;
    u16 dur;
} UnkStruct_02304e80_f;

typedef struct UnkStruct_02304e80_p {
    u8 pad0[0x10];
    u32 offset;
} UnkStruct_02304e80_p;

typedef struct UnkStruct_02304e80 {
    u32 flags;
    u8 pad4[0x588 - 4];
    s32 unk588;
    u8 pad58C[4];
    s32 unk590;
    s32 unk594;
    void (*cb)(void);
    s32 timer;
    s32 frame;
    UnkStruct_02304e80_f *frames;
    s32 nframes;
} UnkStruct_02304e80;

typedef struct UnkStruct_02304e80_w {
    u8 pad0[4];
    s32 type;
} UnkStruct_02304e80_w;

typedef struct UnkStruct_02304e80_q {
    u8 pad0[0x1dc];
    UnkStruct_02304e80_w *work;
} UnkStruct_02304e80_q;

extern UnkStruct_02304e80 *data_ov001_0231f54c;
extern UnkStruct_02304e80_p *data_ov001_0231f550[];

BOOL func_ov001_02300c68(void);
UnkStruct_02304e80_q *func_ov001_02300c94(void);
void func_02035704(void *src, u32 size, u32 offset, void *dst);

void func_ov001_02304e80(void) {
    s32 s;
    UnkStruct_02304e80_f *f;
    UnkStruct_02304e80_p *p;

    if (data_ov001_0231f54c->flags & 1) {
        data_ov001_0231f54c->unk594 = 0;
        return;
    }
    s = data_ov001_0231f54c->unk590;
    if (func_ov001_02300c68() && func_ov001_02300c94()->work->type == 5)
        s >>= 1;
    data_ov001_0231f54c->unk588 += s;
    data_ov001_0231f54c->unk594 = s;
    if (data_ov001_0231f54c->cb != NULL)
        data_ov001_0231f54c->cb();
    data_ov001_0231f54c->timer--;
    if (data_ov001_0231f54c->timer > 0)
        return;
    data_ov001_0231f54c->frame++;
    if (data_ov001_0231f54c->frame >= data_ov001_0231f54c->nframes)
        data_ov001_0231f54c->frame = 0;
    f = &data_ov001_0231f54c->frames[data_ov001_0231f54c->frame];
    p = data_ov001_0231f550[f->idx];
    func_02035704((u8 *)p + p->offset + 4, 0xc0, 0, (void *)0x5000000);
    data_ov001_0231f54c->timer = f->dur;
}
