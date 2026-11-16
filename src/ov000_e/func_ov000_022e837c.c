#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
    u8 pad210[0x224 - 0x210];
    s32 unk224;
    s16 unk228;
    s16 unk22A;
    s16 unk22C;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x42E - 0x20C];
    u16 unk42E;
    u8 pad430[0x440 - 0x430];
    void *unk440;
    u8 pad444[0x464 - 0x444];
    u16 unk464;
    u16 unk466;
    s16 unk468;
    u8 pad46A[2];
    s32 unk46C;
} UnkStruct_022e837c;

extern UnkStruct_Boss *data_ov000_02369b78;
extern u16 data_ov000_02363b3c[];
extern s16 data_ov000_02363c24[][4];
extern s16 data_ov000_02363c64[][4];

void func_ov000_022e9b70(UnkStruct_022e837c *s, s32 a, s32 b);
void func_ov000_022e822c(void);

void func_ov000_022e837c(UnkStruct_022e837c *w) {
    UnkStruct_Boss *b = data_ov000_02369b78;

    w->base.unk2E = 0;
    w->unk466 = 0;
    w->unk468 = 0;
    w->unk46C = 0;
    w->unk464 = data_ov000_02363b3c[w->unk42E];
    func_ov000_022e9b70(w, 0, 1);
    b->unk20C |= 0x20;
    b->unk224 = w->base.unk10;
    b->unk228 = data_ov000_02363c24[w->unk42E][w->unk464 - 1];
    b->unk22A = 0;
    b->unk22C = data_ov000_02363c64[w->unk42E][w->unk464 - 1] >> 1;
    w->unk440 = func_ov000_022e822c;
}
