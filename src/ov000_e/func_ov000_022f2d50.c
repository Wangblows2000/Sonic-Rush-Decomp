#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x48];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad54[0x78 - 0x54];
    s32 unk78;
    u8 pad7C[0x84 - 0x7C];
    s32 unk84;
    u8 pad88[0x8E - 0x88];
    u16 unk8E;
} UnkStruct_Cam;

typedef struct {
    u8 pad[4];
    s32 unk4;
} UnkStruct_02369ac0;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x434 - 0x20C];
    u16 unk434;
    u16 unk436;
    u16 unk438;
} UnkStruct_022f2d50;

extern UnkStruct_Cam data_ov000_0236a48c;
extern UnkStruct_02369ac0 gBossStatus;

void func_ov000_022f4cbc(UnkStruct_022f2d50 *s);
void BossFPartD8_State_Unk_ov000_022f2cf4(void);

void func_ov000_022f2d50(UnkStruct_022f2d50 *s) {
    UnkStruct_Cam *cam;

    s->base.unk2E = 0;
    s->unk434 = 0;
    s->unk436 = 0;
    s->unk438 = 0;
    func_ov000_022f4cbc(s);
    s->base.unk5C &= ~1;
    cam = (UnkStruct_Cam *)(u32)&data_ov000_0236a48c;
    cam->unk48 = 0x64000;
    cam->unk4C = 0x50000;
    cam->unk50 = 0x50000;
    cam->unk78 = 0x1f4000;
    cam->unk84 = -0x28000;
    cam->unk8E = 0xd555;
    gBossStatus.unk4 = 0x32;
    s->base.unk68 = BossFPartD8_State_Unk_ov000_022f2cf4;
}
