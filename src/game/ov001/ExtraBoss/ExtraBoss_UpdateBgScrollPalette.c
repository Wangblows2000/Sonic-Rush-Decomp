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

BOOL ExtraBoss_IsPlayerSpawned(void);
UnkStruct_02304e80_q *ExtraBoss_GetPlayer(void);
void PaletteTransfer_QueueCopy(void *src, u32 size, u32 offset, void *dst);

void ExtraBoss_UpdateBgScrollPalette(void) {
    s32 s;
    UnkStruct_02304e80_f *f;
    UnkStruct_02304e80_p *p;

    if (data_ov001_0231f54c->flags & 1) {
        data_ov001_0231f54c->unk594 = 0;
        return;
    }
    s = data_ov001_0231f54c->unk590;
    if (ExtraBoss_IsPlayerSpawned() && ExtraBoss_GetPlayer()->work->type == 5)
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
    PaletteTransfer_QueueCopy((u8 *)p + p->offset + 4, 0xc0, 0, (void *)0x5000000);
    data_ov001_0231f54c->timer = f->dur;
}
