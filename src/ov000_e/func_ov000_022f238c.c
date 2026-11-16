#include <ov000_c/stageobj.h>

typedef struct {
    u32 unk0;
    u8 pad[0x10 - 4];
    s32 unk10;
    s32 unk14;
} UnkStruct_022c4560;

typedef struct {
    u8 pad[4];
    s32 unk4;
    u16 unk8;
} UnkStruct_02369ac0;

typedef struct {
    u8 pad[0x20C];
    u32 unk20C;
} UnkStruct_Mgr;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    u16 unk434;
    u16 unk436;
} UnkStruct_022f238c;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern UnkStruct_Mgr *data_ov000_02369b84;
extern u16 data_ov000_023641e8[][8][3];

void func_ov000_022f4850(void);
void func_ov000_022f228c(void);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return data_022c4560.unk14;
}

void func_ov000_022f238c(UnkStruct_022f238c *s) {
    UnkStruct_Mgr *m = data_ov000_02369b84;

    s->unk434 = data_ov000_023641e8[GetIdxB()][data_ov000_02369ac0.unk8 - 1][s->unk436];
    func_ov000_022f4850();
    m->unk20C |= 0x40;
    data_ov000_02369ac0.unk4 = 0x33;
    s->base.unk68 = func_ov000_022f228c;
}
