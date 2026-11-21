#include <nitro/types.h>

typedef struct UnkStruct_022cd574_c {
    u8 pad[0x30];
    u16 unk30;
    u8 pad1[0x78 - 0x32];
    s32 unk78;
    u8 pad2[0x80 - 0x7c];
    u16 unk80;
    u8 pad3[0x84 - 0x82];
    s32 unk84;
    u8 pad4[0x8c - 0x88];
    u16 unk8C;
} UnkStruct_022cd574_c;

extern UnkStruct_022cd574_c data_ov000_0236a48c;
extern void *gBoss2WaterLightTask;

void *Task_Create(void (*fn)(void), void (*fn2)(void), s32 b, s32 c, s32 d, s32 e);
void Boss2WaterLight_Main_RestoreColors(void);
void Boss2WaterLight_Destructor(void);

void Boss2WaterLight_CreateRestore(void) {
    UnkStruct_022cd574_c *c = (UnkStruct_022cd574_c *)(u32)&data_ov000_0236a48c;
    c->unk30 = 0x800;
    c->unk78 = 0x1dd2f0;
    c->unk80 = 0x800;
    c->unk84 = 0;
    c->unk8C = 0x800;
    gBoss2WaterLightTask = Task_Create(Boss2WaterLight_Main_RestoreColors, Boss2WaterLight_Destructor, 0, 0x500, 2, 0);
}
