#include <nitro/types.h>

typedef struct { s32 pos; s32 arg; } UnkStruct_023030e4_e;
typedef struct { u8 pad[0x137c]; s32 idx; s32 pos; UnkStruct_023030e4_e *tbl; } UnkStruct_0231f548;
extern UnkStruct_0231f548 *data_ov001_0231f548;
void ExtraBoss_Unk_ov001_02303fd4(s32 a, s32 b);

void ExtraBoss_Unk_ov001_023030e4(void) {
    data_ov001_0231f548->pos += 0x4000;
    for (;;) {
        s32 idx;
        UnkStruct_023030e4_e *t;
        s32 pos;
        t = data_ov001_0231f548->tbl;
        idx = data_ov001_0231f548->idx;
        pos = data_ov001_0231f548->pos;
        if (pos < t[idx].pos) {
            return;
        }
        ExtraBoss_Unk_ov001_02303fd4(t[idx].pos + 0x188000 - pos, t[idx].arg);
        data_ov001_0231f548->idx++;
        {
            UnkStruct_0231f548 *g = data_ov001_0231f548;
            if (g->tbl[g->idx].pos == -1) {
                g->idx = 0;
                data_ov001_0231f548->pos -= 0xe00000;
            }
        }
    }
}
