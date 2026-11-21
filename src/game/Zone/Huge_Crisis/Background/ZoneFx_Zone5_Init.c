#include <nitro/types.h>

typedef struct {
    u8 pad0[0x84];
    void *unk84;
} UnkStruct_0208922c;

typedef struct {
    u8 pad0[0xc];
} UnkEnt_0233e1c4;

typedef struct {
    u8 pad0[8];
} UnkTbl_0233e1c4;

typedef struct {
    u8 pal0[0x204];
    u8 pal1[0x200];
    u8 pal2[0x204];
    UnkEnt_0233e1c4 ent[2];
} UnkStruct_0233e1c4;

extern UnkStruct_0208922c gDisplayMain;
extern UnkTbl_0233e1c4 data_ov000_02364dd0[];
s32 GX_ResetBankForBGExtPltt(void);
void GX_SetBankForBGExtPltt(s32 a);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void ZoneFx_Zone5_TaskMain(void);
void ZoneFx_Zone5_TaskDestructor(void);
void Task_Create(void *a, void *b, s32 c, s32 d, s32 e, s32 f);
void ZoneFx_InitPaletteAnim(UnkEnt_0233e1c4 *e, UnkTbl_0233e1c4 *t);

void ZoneFx_Zone5_Init(UnkStruct_0233e1c4 *w) {
    s32 s;
    u8 i;
    UnkTbl_0233e1c4 *t;

    s = GX_ResetBankForBGExtPltt();
    MIi_CpuCopy32(gDisplayMain.unk84, w->pal1, 0x200);
    GX_SetBankForBGExtPltt(s);
    MIi_CpuCopy32((void *)0x5000000, w->pal2, 0x200);
    MIi_CpuCopy32((void *)0x5000000, w->pal0, 0x200);
    Task_Create(ZoneFx_Zone5_TaskMain, ZoneFx_Zone5_TaskDestructor, 0, 0xf014, 3, 0);
    t = data_ov000_02364dd0;
    for (i = 0; i < 2; i++)
        ZoneFx_InitPaletteAnim(&w->ent[i], &t[i]);
}
