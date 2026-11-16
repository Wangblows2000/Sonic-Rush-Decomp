#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
} UnkStruct_0234eccc_C;

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
    u8 pad14[0x2E - 0x14];
    s16 unk2E;
    u8 pad30[0x68 - 0x30];
    void (*unk68)(void);
    u8 pad6C[0x330 - 0x6C];
    u32 unk330;
    u8 pad334[0x3E8 - 0x334];
    UnkStruct_0234eccc_C *unk3E8;
} UnkStruct_0234eccc;

extern u16 data_022c4ef8;

void func_ov000_0234ec78(void);
void func_ov000_0234c4a4(UnkStruct_0234eccc *w, s32 a);
void BrightnessFade_Create(s32 a, s32 b);

void func_ov000_0234eccc(UnkStruct_0234eccc *w) {
    w->unk330 |= 0xC0;
    if (w->unk3E8 != NULL) {
        w->unk3E8->x = w->unk10 + 0x100;
    }
    w->unkC++;
    if (w->unkC > 0x12) {
        w->unkC = 0;
        data_022c4ef8 = 0x100;
        w->unk2E = 0;
        BrightnessFade_Create(3, 0x20);
        w->unk68 = func_ov000_0234ec78;
        func_ov000_0234c4a4(w, 0x2F);
    }
}
