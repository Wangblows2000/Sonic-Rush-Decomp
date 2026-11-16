#include <nitro/types.h>
#include "main_a2/oammgr.h"

void MIi_CpuCopy16(const void *src, void *dst, u32 size);
void MIi_CpuClear16(u16 data, void *dst, u32 size);

void func_02036a0c(u32 idx) {
    UnkOamMgr *m = data_02072c78[idx];
    UnkOam *dst = m->buf.oam;
    u32 i;
    for (i = 0; i < 32; i++) {
        u16 k = m->heads[i];
        while (k != 0xffff) {
            MIi_CpuCopy16(&m->objs[k], dst, 6);
            k = m->objs[k].next;
            dst++;
        }
    }
    m->unk89c = 0;
    m->numObj = 0;
    {
        vu16 fill = 0xffff;
        MIi_CpuClear16(fill, m->heads, sizeof(m->heads));
    }
    {
        vu16 fill = 0xffff;
        MIi_CpuClear16(fill, m->tails, sizeof(m->tails));
    }
}
