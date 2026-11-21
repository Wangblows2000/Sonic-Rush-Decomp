#include <nitro/types.h>

typedef struct {
    u32 flags;
    u8 pad4[4];
    s32 unk8;
} UnkPlayer_0233dd20;

extern UnkPlayer_0233dd20 gCamera;
extern UnkPlayer_0233dd20 data_ov000_0236ab3c;
extern s16 data_0206e470[];

#define SIN_0233dd20(a) data_0206e470[((s32)((u16)(a) & 0xffff) >> 4) * 2]

void ZoneFx_Zone6_Update(s32 *t) {
    UnkPlayer_0233dd20 *p;

    (*t)++;
    p = &gCamera;
    if (!(p->flags & 0x80))
        p->unk8 += (SIN_0233dd20((*t << 7) * 3) << 10) >> 12;
    p = &data_ov000_0236ab3c;
    if (!(p->flags & 0x80))
        p->unk8 += (SIN_0233dd20((*t << 7) * 3) << 10) >> 12;
}
