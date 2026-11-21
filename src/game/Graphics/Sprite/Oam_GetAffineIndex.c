#include <nitro/types.h>
#include "game/Graphics/oammgr.h"

u16 Oam_GetAffineIndex(u32 idx, const s32 *mtx) {
    UnkOamMgr *m = gDisplayTable[idx];
    if (mtx != NULL) {
        s16 a = mtx[0] >> 4;
        s16 b = mtx[1] >> 4;
        s16 c = mtx[2] >> 4;
        s16 d = mtx[3] >> 4;
        u16 i;
        for (i = 0; i < m->numAffine + m->numObj; i++) {
            UnkOamAffine *e = &m->buf.affine[i];
            if (e->pa == a && e->pb == b && e->pc == c && e->pd == d) {
                return i;
            }
        }
    }
    {
        u16 n = m->numObj;
        if (m->numAffine + n >= 32) {
            return 0xffff;
        }
        m->numObj = n + 1;
        return m->numAffine + n;
    }
}
