#include <nitro/types.h>

typedef struct {
    u16 attr0;
    u16 attr1;
    u16 attr2;
    u16 attr3;
} UnkOam_02038694;

typedef struct {
    u16 attr[3];
    u16 next;
} UnkEnt_02038694;

typedef struct {
    u8 pad0[0x89c];
    u16 unk89c;
    u16 unk89e;
    u16 unk8a0;
    u8 pad8a2[6];
    UnkOam_02038694 oam[128];
    UnkOam_02038694 buf[128];
    UnkEnt_02038694 ent[128];
    u16 head[32];
} UnkMgr_02038694;

extern UnkMgr_02038694 *gDisplayTable[];

void MIi_CpuCopy16(const void *src, void *dst, u32 size);

static inline void CopyAttr3_02038694(UnkOam_02038694 *d, const UnkOam_02038694 *s)
{
    d->attr3 = s->attr3;
}

void Oam_Freeze(s32 screen)
{
    UnkMgr_02038694 *m = gDisplayTable[screen];
    UnkOam_02038694 *dst;
    UnkOam_02038694 *src;
    u32 i;
    u16 idx;

    dst = &m->buf[0x7f - m->unk89c];
    for (i = 0; i < 32; i++) {
        for (idx = m->head[i]; idx != 0xffff; idx = m->ent[idx].next) {
            MIi_CpuCopy16(&m->ent[idx], dst, 6);
            dst++;
        }
    }
    dst = m->buf;
    src = m->oam;
    for (i = 0; i < m->unk89e; i++) {
        CopyAttr3_02038694(dst++, src++);
        CopyAttr3_02038694(dst++, src++);
        CopyAttr3_02038694(dst++, src++);
        CopyAttr3_02038694(dst++, src++);
    }
    m->unk8a0 = m->unk89e;
}
