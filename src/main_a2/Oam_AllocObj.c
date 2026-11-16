#include <nitro/types.h>
#include "main_a2/oammgr.h"

void *func_02006abc(u32 arg);

UnkOamObj *Oam_AllocObj(u32 idx, u32 prio) {
    UnkOamMgr *m = gDisplayTable[idx];
    if (prio >= 32) {
        prio = 31;
    }
    if (m->unk89c >= 128) {
        return func_02006abc(0);
    }
    m->objs[m->unk89c].next = 0xffff;
    if (m->heads[prio] == 0xffff) {
        m->heads[prio] = m->unk89c;
    } else {
        m->objs[m->tails[prio]].next = m->unk89c;
    }
    m->tails[prio] = m->unk89c;
    return &m->objs[m->unk89c++];
}
