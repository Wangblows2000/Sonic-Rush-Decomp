#include <nitro/types.h>

typedef struct UnkStruct_022f4634_o {
    u8 pad0[0x10c];
    u8 unk10C;
} UnkStruct_022f4634_o;

typedef struct UnkStruct_022f4634 {
    u8 pad0[0x74];
    void (*unk74)(void);
    u8 pad78[0x84 - 0x78];
    UnkStruct_022f4634_o *unk84;
    u8 pad88[0x1c8 - 0x88];
    s16 unk1C8;
    u8 pad1CA[2];
    s16 unk1CC;
} UnkStruct_022f4634;

typedef struct UnkStruct_022f4634_g {
    u32 flags;
    u8 pad4[0x40 - 4];
    s32 unk40;
    u8 pad44[0x318 - 0x44];
    s32 unk318;
} UnkStruct_022f4634_g;

extern s32 data_ov001_0231f070;
extern void *gExtraBossArchive;
extern s32 data_ov001_0231d518[];
extern u16 data_ov001_02316918[];
extern u16 data_ov001_02316924[];
extern u16 data_ov001_02316930[];

s32 ExtraBoss_Unk_ov001_022f71f4(void);
void BrightnessLerp_Create(s32 a, s32 b, s32 c, s32 d, void *e);
UnkStruct_022f4634_g *ExtraBoss_GetWork(void);
void ExtraBoss_Unk_ov001_022f41c4(UnkStruct_022f4634 *p);
void ExtraBoss_Unk_ov001_022ffb9c(s32 a, void *b, s32 *c, u16 *d, u16 *e, s32 n);

void ExtraBossObj_State_Unk_ov001_022f4634(UnkStruct_022f4634 *p) {
    UnkStruct_022f4634_o *o = p->unk84;

    ExtraBoss_Unk_ov001_022f71f4();
    p->unk1C8++;
    p->unk1CC += 0x43;
    if (p->unk1CC > 0x1000)
        p->unk1CC = 0x1000;
    if (p->unk1C8 == 0x77)
        BrightnessLerp_Create(0x10, 0, 0x10, 3, 0);
    if (o->unk10C & 2) {
        if (ExtraBoss_GetWork()->unk40 == 4) {
            ExtraBoss_GetWork()->flags |= 0x100;
            p->unk74 = 0;
            return;
        }
        ExtraBoss_Unk_ov001_022f41c4(p);
        ExtraBoss_GetWork()->unk318 = 0;
        ExtraBoss_Unk_ov001_022ffb9c(data_ov001_0231f070, gExtraBossArchive, data_ov001_0231d518, data_ov001_02316918, data_ov001_02316924, 5);
        return;
    }
    if ((p->unk1C8 & 7) == 1)
        ExtraBoss_Unk_ov001_022ffb9c(data_ov001_0231f070, gExtraBossArchive, data_ov001_0231d518, data_ov001_02316930, data_ov001_02316924, 5);
    else if ((p->unk1C8 & 7) == 5)
        ExtraBoss_Unk_ov001_022ffb9c(data_ov001_0231f070, gExtraBossArchive, data_ov001_0231d518, data_ov001_02316918, data_ov001_02316924, 5);
}
