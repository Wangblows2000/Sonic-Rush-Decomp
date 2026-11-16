#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s32 unk10;
    s32 unk14;
    s32 unk18;
    u8 pad1C[0x28 - 0x1C];
    s32 unk28;
    s32 unk2C;
    s32 unk30;
    s32 unk34;
    s32 unk38;
    s32 unk3C;
    u8 pad40[4];
    s32 unk44;
    s32 unk48;
    u8 unk4C[0x184 - 0x4C];
    void *unk184;
    void *unk188;
    void *unk18C;
    u8 pad190[0x198 - 0x190];
    s32 unk198;
    s32 unk19C;
    u8 pad1A0[4];
    s32 unk1A4;
    s32 unk1A8;
    u8 unk1AC[0x2B8 - 0x1AC];
    u8 unk2B8;
    u8 pad2B9[0x2E4 - 0x2B9];
    void *unk2E4;
    void *unk2E8;
    void *unk2EC;
    u8 pad2F0[0x2FC - 0x2F0];
    void *unk2FC;
    u8 pad300[4];
} UnkStruct_02357148;

extern void *data_ov000_02368dfc;
extern void *data_ov000_0236ac50[];
extern u8 data_ov000_023690d0[];
extern u8 data_ov000_02364eb8[];
extern u16 data_ov000_02364eb0[];

void MIi_CpuClear32(u32 data, void *dest, u32 size);
void *File_Load(void *a, s32 b);
void func_02021310(void *f, void *name, void *heap);
void *func_02021264(void *f, s32 idx);
void func_020212e0(void *f);
void Model_Init(void *o, s32 a);
void Model_SetResource(void *o, void *r, s32 a);
void Model_SetAnimation(void *o, s32 a, void *r, s32 b, s32 c);
void G3X_SetEdgeColorTable(void *a);
void func_02022270(s32 a, u16 b);

#pragma thumb on

void PlayerSelectMenu_LoadModels(UnkStruct_02357148 *w) {
    u32 file[0x5c / 4];
    vu32 zero;

    zero = 0;
    MIi_CpuClear32(zero, w, sizeof(UnkStruct_02357148));
    w->unk2FC = File_Load(data_ov000_02368dfc, -1);
    func_02021310(file, data_ov000_023690d0, w->unk2FC);
    w->unk184 = data_ov000_0236ac50[0];
    w->unk188 = func_02021264(file, 0);
    w->unk18C = func_02021264(file, 1);
    w->unk2E4 = data_ov000_0236ac50[2];
    w->unk2E8 = func_02021264(file, 0);
    w->unk2EC = func_02021264(file, 1);
    func_020212e0(file);
    Model_Init(w->unk4C, 0);
    Model_SetResource(w->unk4C, w->unk184, 0);
    Model_SetAnimation(w->unk4C, 0, w->unk188, 0, 0);
    Model_SetAnimation(w->unk4C, 4, w->unk18C, 0, 0);
    Model_Init(w->unk1AC, 0);
    Model_SetResource(w->unk1AC, w->unk2E4, 0);
    Model_SetAnimation(w->unk1AC, 0, w->unk2E8, 4, 0);
    Model_SetAnimation(w->unk1AC, 4, w->unk2EC, 4, 0);
    w->unk0 = 0;
    w->unk4 = 0;
    w->unk8 = 0;
    w->unkC = 0x1a00;
    w->unk10 = 0;
    w->unk14 = 0;
    w->unk18 = 0;
    w->unk28 = 0;
    w->unk2C = 0;
    w->unk30 = 0;
    w->unk34 = 0;
    w->unk38 = 0;
    w->unk3C = -1;
    w->unk44 = 0xb1c7;
    w->unk48 = 0;
    w->unk198 = 4;
    w->unk19C = -1;
    w->unk1A4 = 0xae38;
    w->unk1A8 = 0;
    w->unk2B8 |= 4;
    w->unk2B8 |= 4;
    G3X_SetEdgeColorTable(data_ov000_02364eb8);
    func_02022270(0, data_ov000_02364eb0[0]);
    func_02022270(1, data_ov000_02364eb0[1]);
    func_02022270(2, data_ov000_02364eb0[2]);
}

#pragma thumb reset
