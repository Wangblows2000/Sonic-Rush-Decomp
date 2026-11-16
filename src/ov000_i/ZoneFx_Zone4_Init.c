#include <nitro/types.h>

typedef struct {
    u8 pad0[0x84];
    void *unk84;
} UnkStruct_0208922c;

typedef struct {
    u8 pad0[0xc];
} UnkEnt_0233e290;

typedef struct {
    u8 pad0[8];
} UnkTbl_0233e290;

typedef struct {
    u8 pad0[0x10];
    u8 pal1[0x200];
    u8 pal2[0x200];
    UnkEnt_0233e290 ent[24];
} UnkStruct_0233e290;

typedef struct {
    u32 flags;
    u8 pad4[0x64 - 4];
} UnkPlayer_0233e290;

extern UnkStruct_0208922c gDisplayMain;
extern UnkTbl_0233e290 data_ov000_02364df0[];
s32 GX_ResetBankForBGExtPltt(void);
void GX_SetBankForBGExtPltt(s32 a);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void ZoneFx_SetBrightnessBlend(s32 a);
extern UnkPlayer_0233e290 gCamera[];
void ZoneFx_InitPaletteAnim(UnkEnt_0233e290 *e, UnkTbl_0233e290 *t);

void ZoneFx_Zone4_Init(UnkStruct_0233e290 *w) {
    s32 s;
    u8 i;
    UnkTbl_0233e290 *t;

    gCamera[2].flags |= 0x100000;
    ZoneFx_SetBrightnessBlend(0);
    s = GX_ResetBankForBGExtPltt();
    MIi_CpuCopy32(gDisplayMain.unk84, w->pal1, 0x200);
    GX_SetBankForBGExtPltt(s);
    MIi_CpuCopy32((void *)0x5000000, w->pal2, 0x200);
    t = data_ov000_02364df0;
    for (i = 0; i < 24; i++)
        ZoneFx_InitPaletteAnim(&w->ent[i], &t[i]);
}
