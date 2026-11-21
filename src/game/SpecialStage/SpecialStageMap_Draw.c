#include <nitro/types.h>

typedef struct UnkStruct_0230d708_m {
    s32 m[12];
} UnkStruct_0230d708_m;

typedef struct UnkStruct_0230d708_e {
    s32 type;
    UnkStruct_0230d708_m mtx;
    u16 flags;
    u8 pad36[2];
    struct UnkStruct_0230d708_e *next;
} UnkStruct_0230d708_e;

typedef struct UnkStruct_0230d708 {
    u8 pad0[8];
    UnkStruct_0230d708_m mtx;
    u8 pad38[0x8c4 - 0x38];
    u8 obj8C4[0xa48 - 0x8c4];
    s32 type;
    UnkStruct_0230d708_m mtxA4C;
    u8 padA7C[4];
    UnkStruct_0230d708_e *head;
    u8 padA84[4];
    u8 sentinel[4];
} UnkStruct_0230d708;

typedef struct UnkStruct_0230d708_t {
    void *unk0;
    void *unk4;
    void (*fn8)(void *p);
    void (*fnC)(UnkStruct_0230d708_m *m);
    void *unk10;
    void (*fn14)(UnkStruct_0230d708_m *m);
} UnkStruct_0230d708_t;

extern UnkStruct_0230d708 *gSpecialStageMap;
extern UnkStruct_0230d708_t gSpecialStageMapSegmentTypeTable[];
extern UnkStruct_0230d708_t data_ov001_0231722c[];
extern UnkStruct_0230d708_t data_ov001_02317238[];

BOOL SpecialStageMap_IsSegmentListEmpty(void);
void NNS_G3dGeBufferOP_N(u32 op, const u32 *args, u32 n);
void Model_Draw(void *o);

void SpecialStageMap_Draw(void) {
    UnkStruct_0230d708 *g = gSpecialStageMap;
    UnkStruct_0230d708_e *e;
    u32 a;
    u32 one;
    u32 b;
    u32 one2;
    UnkStruct_0230d708_m m;
    UnkStruct_0230d708_m m2;

    if (SpecialStageMap_IsSegmentListEmpty())
        return;
    m = g->mtx;
    ((void (*)(UnkStruct_0230d708_m *))data_ov001_02317238[g->type].unk0)(&m);
    g->mtxA4C = m;
    NNS_G3dGeBufferOP_N(0x15, 0, 0);
    NNS_G3dGeBufferOP_N(0x11, 0, 0);
    NNS_G3dGeBufferOP_N(0x19, (u32 *)&m, 0xc);
    a = 0x1e;
    NNS_G3dGeBufferOP_N(0x13, &a, 1);
    ((void (*)(void *))data_ov001_0231722c[g->type].unk0)(&g->type);
    one = 1;
    NNS_G3dGeBufferOP_N(0x12, &one, 1);
    for (e = g->head; e != (UnkStruct_0230d708_e *)g->sentinel; e = e->next) {
        if (e->flags & 1)
            continue;
        m2 = e->mtx;
        gSpecialStageMapSegmentTypeTable[e->type].fnC(&m2);
        NNS_G3dGeBufferOP_N(0x15, 0, 0);
        NNS_G3dGeBufferOP_N(0x11, 0, 0);
        NNS_G3dGeBufferOP_N(0x19, (u32 *)&m2, 0xc);
        b = 0x1e;
        NNS_G3dGeBufferOP_N(0x13, &b, 1);
        gSpecialStageMapSegmentTypeTable[e->type].fn8(e);
        one2 = 1;
        NNS_G3dGeBufferOP_N(0x12, &one2, 1);
    }
    Model_Draw(g->obj8C4);
}
