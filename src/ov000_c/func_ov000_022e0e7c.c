#include <ov000_c/stageobj.h>

typedef struct {
  u32 unk0;
  s32 unk4;
  u16 unk8;
  u16 unkA;
} UnkStruct_02369ac0;

typedef struct {
  u8 pad0[0x10C];
  u8 unk10C;
  u8 pad10D[0x114 - 0x10D];
  s32 unk114;
} UnkStruct_Mdl960;

typedef struct {
  UnkStruct_StageObj base;
  u32 unk20C;
  u8 pad210[0x960 - 0x210];
  UnkStruct_Mdl960 unk960;
  u8 pad[0xD08 - 0x960 - sizeof(UnkStruct_Mdl960)];
  void *unkD08;
  u8 padD0C[0xD60 - 0xD0C];
  UnkStruct_Coll unkD60[11];
  u8 padF54[0x1118 - 0xD60 - 11 * 0x2C];
  s32 unk1118;
  s32 unk111C;
} UnkStruct_Boss;

typedef struct {
  s32 unk0;
  s32 unk4;
  u8 pad8[0x18 - 8];
  UnkStruct_StageObj *unk18;
} UnkStruct_Hit;

extern UnkStruct_02369ac0 data_ov000_02369ac0;
extern s32 data_ov000_02363718[];

void func_ov000_022e2470(void);
void func_ov000_022e04a8(UnkStruct_Boss *w);
void func_ov000_022df9d4(s32 a);
void func_02031a68(void *s, s32 a, void *b, s32 c, s32 d);
void func_ov000_02328710(s32 a, s32 x, s32 y, s32 z);
void func_020537b4(s32 a);
void func_020533e0(s32 a, s32 b);
void func_020367b4(s32 a, s32 b);
void func_ov000_0234aa80(UnkStruct_StageObj *p);
void func_02036754(s32 a, s32 b, s32 c);
void func_020366f4(s32 a, s32 b, s32 c);
void func_ov000_022e4718(void);

void func_ov000_022e0e7c(UnkStruct_Coll *a, UnkStruct_Hit *b) {
  UnkStruct_Boss *w = a->unk18;
  UnkStruct_Mdl960 *m = &w->unk960;
  s32 i;

  if (data_ov000_02369ac0.unk8 != 0) {
    data_ov000_02369ac0.unk8--;
    for (i = 0; i < 11; i++) {
      w->unkD60[i].unkA &= ~4;
      w->unkD60[i].unkA |= 0x1000;
    }
    w->unk20C &= ~4;
    if (data_ov000_02369ac0.unk8 != 0) {
      w->base.unk68 = func_ov000_022e4718;
      func_ov000_022e04a8(w);
      w->unk111C = 8;
      func_ov000_022df9d4(0);
      w->unk1118 = 0;
      func_02031a68(m, 0, w->unkD08, 6, 0);
      m->unk10C &= ~4;
      m->unk114 = 0x1000;
      data_ov000_02369ac0.unk4 = 0x22;
      w->unk20C |= 0x200;
      func_ov000_02328710(0, b->unk0 << 8, b->unk4 << 8, 0);
      if (data_ov000_02369ac0.unk8 == 2) {
        func_020537b4(0xb);
        func_020533e0(0x12, 0xf0);
        func_020367b4(0, 0xf);
      } else {
        func_020537b4(0xb);
        func_020533e0(0x10, 0xf0);
      }
      if (b->unk18 != NULL && b->unk18->unk0 == 1)
        func_ov000_0234aa80(b->unk18);
      func_02036754(0, 0, 0x7b);
      func_020366f4(0, 0, data_ov000_02363718[Rand() & 1]);
    } else {
      func_ov000_022e2470();
    }
  } else {
    func_ov000_022e2470();
  }
}
