#include <nitro/types.h>

typedef struct UnkStruct_0230c104_br {
    u8 pad0[0x20];
    u16 b0 : 1;
    u16 p1 : 5;
    u16 b6 : 2;
    u16 p8 : 1;
    u16 b9 : 1;
    u16 b10 : 1;
    u16 p11 : 1;
    u16 b12 : 1;
    u16 p13 : 3;
    u16 e0 : 5;
    u16 p5 : 3;
    u16 e8 : 5;
    u16 p13b : 3;
} UnkStruct_0230c104_br;

typedef struct UnkStruct_0230c104_p {
    s16 x;
    s16 y;
} UnkStruct_0230c104_p;

typedef struct UnkStruct_0230c104 {
    u8 pad0[4];
    s32 unk4;
    u8 pad8[4];
    void *taskC;
    void *task10;
    void *task14;
    u8 *unk18;
    u8 *unk1C;
    u8 pad20[4];
    s32 unk24;
} UnkStruct_0230c104;

typedef struct UnkStruct_0230c104_4560 {
    u8 pad0[0x28];
    u16 unk28;
} UnkStruct_0230c104_4560;

extern UnkStruct_0230c104 *gSpecialStageWork;
extern void *data_ov001_0231f5c4;
extern void *gMainHeap;
extern UnkStruct_0230c104_4560 data_022c4560;
extern UnkStruct_0230c104_br gDisplayMain;
extern UnkStruct_0230c104_p gDisplaySub[];

void Task_Destroy(void *t);
void func_ov001_0230f77c(u8 *blk);
void func_ov001_02313900(u8 *blk);
void func_ov001_0230b094(void);
void func_ov001_023071fc(void);
void func_ov001_0230f8b4(void);
void func_ov001_0230db5c(void);
void func_ov001_02313b58(void);
void func_ov001_02315dc0(void);
void func_ov001_0231444c(void);
void func_ov001_0230a438(void);
void func_ov001_023147dc(void);
void func_ov001_0230bcc4(void *p);
void *func_02006abc(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void Heap_Free(void *heap, void *p);
void func_0200412c(BOOL enable, u32 fogMode, u32 fogSlope, s32 fogOffset);

static inline void Free_0230c104(void *p) {
    u32 size;
    if (func_02006abc(0) != p) {
        p = (u8 *)p - 0x20;
        size = *(u32 *)p;
        if (*(u32 *)((u8 *)p + size - 4) == 0x2467531) {
            vu32 fill = 0xcccccccc;
            MIi_CpuClear32(fill, p, size);
            DC_StoreRange(p, size);
        }
    }
    Heap_Free(gMainHeap, p);
}

void SpecialStage_Shutdown(void) {
    UnkStruct_0230c104 *g = gSpecialStageWork;
    s32 i;

    if (g->taskC != NULL)
        Task_Destroy(g->taskC);
    if (g->task10 != NULL)
        Task_Destroy(g->task10);
    if (g->task14 != NULL)
        Task_Destroy(g->task14);
    func_ov001_0230f77c(g->unk1C);
    if (g->unk18 != NULL) {
        func_ov001_02313900(g->unk18);
        g->unk18 = NULL;
    }
    func_ov001_0230b094();
    func_ov001_023071fc();
    func_ov001_0230f8b4();
    func_ov001_0230db5c();
    func_ov001_02313b58();
    func_ov001_02315dc0();
    func_ov001_0231444c();
    func_ov001_0230a438();
    func_ov001_023147dc();
    func_ov001_0230bcc4(data_ov001_0231f5c4);
    Free_0230c104(data_ov001_0231f5c4);
    data_ov001_0231f5c4 = NULL;
    if (g->unk24 != 0)
        data_022c4560.unk28 |= 1 << g->unk4;
    Free_0230c104(gSpecialStageWork);
    gSpecialStageWork = NULL;
    gDisplayMain.b0 = 0;
    gDisplayMain.b9 = 0;
    gDisplayMain.e0 = 0;
    gDisplayMain.b10 = 0;
    gDisplayMain.b12 = 0;
    gDisplayMain.b6 = 0;
    gDisplayMain.e8 = 0;
    func_0200412c(0, 0, 4, 0);
    for (i = 0; i < 4; i++) {
        ((UnkStruct_0230c104_p *)&gDisplayMain)[i].x = 0;
        ((UnkStruct_0230c104_p *)&gDisplayMain)[i].y = 0;
        gDisplaySub[i].x = 0;
        gDisplaySub[i].y = 0;
    }
}
