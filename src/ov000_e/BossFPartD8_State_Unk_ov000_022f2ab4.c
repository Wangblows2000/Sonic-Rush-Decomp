#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x2BC - 0x20C];
    UnkStruct_Coll coll2BC;
    u8 pad2E8[0x434 - 0x2E8];
    u16 unk434;
    u8 pad436[0x458 - 0x436];
    s32 unk458;
} UnkStruct_022f2ab4;

extern s16 data_ov000_0236415c[][5];

void BossFPartD8_SetAnimation(UnkStruct_022f2ab4 *s, s32 a, s32 b, s32 c);
void ObjRect_SetBox3D(UnkStruct_Coll *c, s16 a, s16 b, s16 d, s16 e, s16 f, s16 g);
void BossFPartD8_State_Unk_ov000_022f29a8(void);

void BossFPartD8_State_Unk_ov000_022f2ab4(UnkStruct_022f2ab4 *s) {
    BossFPartD8_SetAnimation(s, 0x16, data_ov000_0236415c[s->unk458][s->unk434], 0);
    s->coll2BC.unkA |= 4;
    ObjRect_SetBox3D(&s->coll2BC, 0x10, -0x30, 0x70, 0x38, -0x7d00, 0x7d00);
    s->base.unk68 = BossFPartD8_State_Unk_ov000_022f29a8;
}
