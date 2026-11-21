#include <nitro/types.h>
#include "game/Graphics/oammgr.h"

void MIi_CpuClear16(u16 data, void *dst, u32 size);

static inline void Fill16(void *dst, u16 v, u32 size) {
    vu16 f = v;
    MIi_CpuClear16(f, dst, size);
}

static inline void Clear16(void *dst, u32 size) {
    Fill16(dst, 0, size);
}

void Oam_Init(void) {
    u32 i;
    for (i = 0; i < 2; i++) {
        UnkOamMgr *m = gDisplayTable[i];
        m->unk89c = 0;
        m->numObj = 0;
        m->numAffine = 0;
        m->unk8a2 = 0;
        m->unk8a4 = 0;
        Fill16(m->buf.oam, 0x200, sizeof(m->buf));
        Fill16(m->unkca8, 0x200, sizeof(m->unkca8));
        Clear16(m->objs, sizeof(m->objs));
        Fill16(m->heads, 0xffff, sizeof(m->heads));
        Fill16(m->tails, 0xffff, sizeof(m->tails));
    }
}
