#include <ov000_c/stageobj.h>

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
    UnkStruct_StageObj base;
    u8 pad20C[0x430 - 0x20C];
    u16 unk430;
    u8 pad432[0x434 - 0x432];
    u16 unk434;
} UnkStruct_022f228c;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern u8 data_ov000_02363edc[][8];
extern u16 data_ov000_02363fdc[][8];

void func_ov000_022f4c40(UnkStruct_022f228c *s);
void func_ov000_022f89f4(void);
void func_ov000_022f21f0(void);

static inline s32 GetIdxB(void) {
    BOOL b = TRUE;
    if (data_022c4560.unk10 != 2 && data_022c4560.unk10 != 3)
        b = FALSE;
    if (b)
        return 1;
    return data_022c4560.unk14;
}

void func_ov000_022f228c(UnkStruct_022f228c *s) {
    func_ov000_022f4c40(s);
    if (data_ov000_02363edc[GetIdxB()][data_ov000_02369ac0.unk8 - 1] != 0) {
        if (s->unk430-- == 0) {
            func_ov000_022f89f4();
            s->unk430 = data_ov000_02363fdc[GetIdxB()][data_ov000_02369ac0.unk8 - 1];
        }
    }
    s->unk434--;
    if (s->unk434 == 0)
        s->base.unk68 = func_ov000_022f21f0;
}
