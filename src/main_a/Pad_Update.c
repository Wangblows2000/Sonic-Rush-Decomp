#include <nitro/types.h>

typedef struct {
    u16 cur;
    u16 prev;
    u16 trg;
    u16 rel;
    u16 rep;
    u16 cnt[12];
    u16 delay[12];
    u16 interval[12];
} UnkPad_02035134;

void Pad_Update(UnkPad_02035134 *p, u16 keys) {
    s32 i;
    p->prev = p->cur;
    p->cur = keys;
    p->trg = p->cur & (p->cur ^ p->prev);
    p->rel = p->prev & (p->cur ^ p->prev);
    p->rep = p->trg;
    for (i = 0; i < 12; i++) {
        u32 bit = 1 << i;
        if (!(p->cur & bit)) {
            p->cnt[i] = p->delay[i];
        } else if (p->cnt[i] != 0) {
            p->cnt[i]--;
        } else {
            p->rep |= bit;
            p->cnt[i] = p->interval[i];
        }
    }
}
