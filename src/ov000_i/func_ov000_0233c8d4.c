#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u8 base;
    u8 frames;
    u8 dst;
    u8 count;
    u16 wait;
} UnkAnim_0233c8d4;

typedef struct {
    u16 *src;
    UnkAnim_0233c8d4 *anim;
    u8 pad8;
    s8 frame;
    u16 timer;
} UnkEnt_0233c8d4;

void MIi_CpuCopy16(const void *src, void *dst, u32 size);

void func_ov000_0233c8d4(UnkEnt_0233c8d4 *e, u16 *pal) {
    UnkAnim_0233c8d4 *a = e->anim;

    e->timer = a->wait;
    e->frame++;
    if (e->frame >= a->frames)
        e->frame = 0;
    MIi_CpuCopy16(&e->src[(e->frame << 4) + a->base], &pal[a->dst], a->count * 2);
}
