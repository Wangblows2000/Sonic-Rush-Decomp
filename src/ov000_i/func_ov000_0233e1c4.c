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
s32 func_0200311c(void);
void func_02003788(s32 a);
void MIi_CpuCopy32(const void *src, void *dst, u32 size);
void func_ov000_0233c778(void);
void func_ov000_0233c790(void);
void Task_Create(void *a, void *b, s32 c, s32 d, s32 e, s32 f);
void func_ov000_0233c934(UnkEnt_0233e1c4 *e, UnkTbl_0233e1c4 *t);

void func_ov000_0233e1c4(UnkStruct_0233e1c4 *w) {
    s32 s;
    u8 i;
    UnkTbl_0233e1c4 *t;

    s = func_0200311c();
    MIi_CpuCopy32(gDisplayMain.unk84, w->pal1, 0x200);
    func_02003788(s);
    MIi_CpuCopy32((void *)0x5000000, w->pal2, 0x200);
    MIi_CpuCopy32((void *)0x5000000, w->pal0, 0x200);
    Task_Create(func_ov000_0233c778, func_ov000_0233c790, 0, 0xf014, 3, 0);
    t = data_ov000_02364dd0;
    for (i = 0; i < 2; i++)
        func_ov000_0233c934(&w->ent[i], &t[i]);
}
