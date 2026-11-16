#include <nitro/types.h>

typedef struct {
    u8 pad0[0x84];
    void *unk84;
} UnkStruct_0208922c;

typedef struct {
    u8 pad0[0xc];
} UnkEnt_0233e138;

typedef struct {
    u8 pad0[8];
} UnkTbl_0233e138;

typedef struct {
    u8 pal0[0x200];
    u8 pal1[0x204];
    UnkEnt_0233e138 ent[2];
} UnkStruct_0233e138;

extern UnkStruct_0208922c gDisplayMain;
extern UnkTbl_0233e138 data_ov000_02364de0[];
s32 GX_ResetBankForBGExtPltt(void);
void GX_SetBankForBGExtPltt(s32 a);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void func_ov000_0233c934(UnkEnt_0233e138 *e, UnkTbl_0233e138 *t);

void func_ov000_0233e138(UnkStruct_0233e138 *w) {
    s32 s;
    u8 i;
    UnkTbl_0233e138 *t;

    s = GX_ResetBankForBGExtPltt();
    MIi_CpuCopy32(gDisplayMain.unk84, w->pal0, 0x200);
    GX_SetBankForBGExtPltt(s);
    MIi_CpuCopy32((void *)0x5000000, w->pal1, 0x200);
    t = data_ov000_02364de0;
    for (i = 0; i < 2; i++)
        func_ov000_0233c934(&w->ent[i], &t[i]);
}
