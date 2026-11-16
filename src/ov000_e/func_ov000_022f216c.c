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
    u16 unk436;
} UnkStruct_022f216c;

extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern u8 data_ov000_02363edc[][8];
extern u16 data_ov000_02363fdc[][8];

typedef struct {
    u8 pad[0x20C];
    u32 unk20C;
} UnkStruct_Mgr;

extern UnkStruct_Mgr *data_ov000_02369b84;

void func_ov000_022f4c40(UnkStruct_022f216c *s);
void func_ov000_022f2014(void);
void func_ov000_022f1b74(void);

void func_ov000_022f216c(UnkStruct_022f216c *s) {
    UnkStruct_Mgr *m = data_ov000_02369b84;

    func_ov000_022f4c40(s);
    s->unk434--;
    if (s->unk434 != 0)
        return;
    s->unk436++;
    if (s->unk436 < 3)
        s->base.unk68 = func_ov000_022f2014;
    else
        s->base.unk68 = func_ov000_022f1b74;
    m->unk20C &= ~0x40;
}
