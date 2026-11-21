#include <nitro/types.h>

typedef struct { s32 pos; s32 arg; } UnkStruct_023030e4_e;
typedef struct { u8 pad[0x137c]; s32 idx; s32 pos; UnkStruct_023030e4_e *tbl; } UnkStruct_0231f548;
extern UnkStruct_0231f548 *gExtraBossRingWork;
void ExtraBoss_PushScrollEntry(s32 a, s32 b);

void ExtraBoss_SpawnFromScrollTable(void) {
    gExtraBossRingWork->pos += 0x4000;
    for (;;) {
        s32 idx;
        UnkStruct_023030e4_e *t;
        s32 pos;
        t = gExtraBossRingWork->tbl;
        idx = gExtraBossRingWork->idx;
        pos = gExtraBossRingWork->pos;
        if (pos < t[idx].pos) {
            return;
        }
        ExtraBoss_PushScrollEntry(t[idx].pos + 0x188000 - pos, t[idx].arg);
        gExtraBossRingWork->idx++;
        {
            UnkStruct_0231f548 *g = gExtraBossRingWork;
            if (g->tbl[g->idx].pos == -1) {
                g->idx = 0;
                gExtraBossRingWork->pos -= 0xe00000;
            }
        }
    }
}
