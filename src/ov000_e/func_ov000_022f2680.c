#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad[0x20C];
    u32 unk20C;
} UnkStruct_Mgr;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    u16 unk434;
    u8 pad436[0x458 - 0x436];
    s32 unk458;
} UnkStruct_022f2680;

extern UnkStruct_Mgr *gBossF;
extern u8 data_ov000_02363df0[];
extern s16 data_ov000_0236415c[][5];

void func_ov000_022f4cbc(UnkStruct_022f2680 *s);
void func_ov000_022f4ce4(UnkStruct_022f2680 *s, s32 a, s32 b, s32 c);
void BossFPartD8_State_Unk_ov000_022f2608(UnkStruct_022f2680 *s);

void func_ov000_022f2680(UnkStruct_022f2680 *s) {
    UnkStruct_Mgr *m = gBossF;
    s32 v;

    func_ov000_022f4cbc(s);
    if (s->unk434 + 1 < data_ov000_02363df0[s->unk458])
        v = data_ov000_0236415c[s->unk458][s->unk434];
    else
        v = 0x1000;
    func_ov000_022f4ce4(s, 0x17, v, 0);
    m->unk20C &= ~0x40;
    s->base.unk1B8 = NULL;
    s->base.unk68 = BossFPartD8_State_Unk_ov000_022f2608;
    ((void (*)(UnkStruct_022f2680 *))s->base.unk68)(s);
}
