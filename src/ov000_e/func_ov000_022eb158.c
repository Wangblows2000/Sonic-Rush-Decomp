#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad[8];
    u16 unk8;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad[0x220];
    u32 unk220;
} UnkStruct_02369b78;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[0x2E - 0x14];
    s16 unk2E;
    u8 pad30[0x328 - 0x30];
    s16 unk328;
} UnkStruct_022eb158;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern UnkStruct_02369b78 *data_ov000_02369b78;
extern u16 data_ov000_02363be4[][8];
extern UnkStruct_022eb158 *data_ov000_0236ac60;

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return data_022c4560.unk14;
}

BOOL func_ov000_022eb158(UnkStruct_022eb158 *s) {
    UnkStruct_022eb158 *p;
    s32 d;
    UnkStruct_02369b78 *g = data_ov000_02369b78;

    if (g->unk220 & data_ov000_02363be4[GetIdxB()][data_ov000_02369ac0.unk8 - 1]) {
        return FALSE;
    }
    p = data_ov000_0236ac60;
    if (p->unk328 != 0x30) {
        return FALSE;
    }
    d = s->unk10 - p->unk10;
    if (d < 0) {
        d = -d;
    }
    if (d < 0x6000) {
        if ((s->unk10 < p->unk10 && p->unk2E < 0) || (s->unk10 > p->unk10 && p->unk2E > 0)) {
            return TRUE;
        }
    }
    return d < 0x3000;
}
