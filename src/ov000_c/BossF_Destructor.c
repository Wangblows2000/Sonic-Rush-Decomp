#include <ov000_c/stageobj.h>

typedef struct {
    u8 pad0[0x138];
} UnkStruct_022f84fc_Sub;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x238 - 0x20C];
    void *unk238;
    void *unk23C;
    void *unk240;
    UnkStruct_022f84fc_Sub unk244;
    UnkStruct_022f84fc_Sub unk37C[4];
    UnkStruct_022f84fc_Sub unk85C;
} UnkStruct_022f84fc;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_022f84fc *work;
} UnkStruct_022f84fc_Task;

typedef struct {
    u8 pad0[0x20];
    struct {
        u16 b0 : 6;
        u16 b6 : 2;
        u16 b8 : 8;
    } unk20;
} UnkStruct_0208922c;

extern void *gZoneActArchive;
extern void *gMainHeap;
extern UnkStruct_0208922c gDisplayMain;
extern u8 data_ov000_02365f38[];
extern u8 data_ov000_02365f3c[], data_ov000_02365f50[], data_ov000_02365f68[], data_ov000_02365f80[], data_ov000_02365f98[], data_ov000_02365fb0[], data_ov000_02365fc8[], data_ov000_02365fe0[], data_ov000_02365ff8[], data_ov000_02366014[], data_ov000_0236602c[];

void func_02021310(void *f, void *path, void *arc);
void *func_0202128c(void *a);
void func_0202566c(void *p);
void func_020212e0(void *f);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 n);
void Heap_Free(void *heap, void *p);
void Model_Release(void *s);
void ScreenShake_Start(s32 a);
void MapObject_Destructor(UnkStruct_022f84fc_Task *t);

static inline void FreeRes(void *p) {
    u8 *q = p;
    u32 size;
    vu32 fill;

    if ((u8 *)func_02006abc(0) != q) {
        q -= 0x20;
        size = *(u32 *)q;
        if (*(u32 *)(q + size - 4) == 0x02467531) {
            fill = 0xcccccccc;
            MIi_CpuClear32(fill, q, size);
            DC_StoreRange(q, size);
        }
    }
    Heap_Free(gMainHeap, q);
}

void BossF_Destructor(UnkStruct_022f84fc_Task *t) {
    u32 ctx[0x17];
    UnkStruct_022f84fc *w = t->work;
    s32 i;
    UnkStruct_022f84fc_Sub *sub;

    func_02021310(ctx, data_ov000_02365f38, gZoneActArchive);
    func_0202566c(func_0202128c(data_ov000_02365f3c));
    func_0202566c(func_0202128c(data_ov000_02365f50));
    func_0202566c(func_0202128c(data_ov000_02365f68));
    func_0202566c(func_0202128c(data_ov000_02365f80));
    func_0202566c(func_0202128c(data_ov000_02365f98));
    func_0202566c(func_0202128c(data_ov000_02365fb0));
    func_0202566c(func_0202128c(data_ov000_02365fc8));
    func_0202566c(func_0202128c(data_ov000_02365fe0));
    func_0202566c(func_0202128c(data_ov000_02365ff8));
    func_0202566c(func_0202128c(data_ov000_02366014));
    func_0202566c(func_0202128c(data_ov000_0236602c));
    func_020212e0(ctx);
    if (w->unk238 != NULL) {
        func_0202566c(w->unk238);
        FreeRes(w->unk238);
        w->unk238 = NULL;
    }
    if (w->unk23C != NULL) {
        func_0202566c(w->unk23C);
        FreeRes(w->unk23C);
        w->unk23C = NULL;
    }
    if (w->unk240 != NULL) {
        func_0202566c(w->unk240);
        FreeRes(w->unk240);
        w->unk240 = NULL;
    }
    Model_Release(&w->unk244);
    Model_Release(&w->unk85C);
    sub = w->unk37C;
    for (i = 0; i < 4; i++) {
        Model_Release(sub);
        sub++;
    }
    gDisplayMain.unk20.b6 = 0;
    ScreenShake_Start(0);
    MapObject_Destructor(t);
}
