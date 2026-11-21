#include <nitro/types.h>

typedef struct UnkStruct_0230d964_m {
    s32 m[12];
} UnkStruct_0230d964_m;

typedef struct UnkStruct_0230d964_e {
    s32 type;
    UnkStruct_0230d964_m mtx;
    u16 flags;
    u8 pad36[2];
    struct UnkStruct_0230d964_e *next;
} UnkStruct_0230d964_e;

typedef struct UnkStruct_0230d964 {
    s32 time;
    s32 step;
    UnkStruct_0230d964_m mtx;
    u8 pad38[0x8c4 - 0x38];
    u8 obj8C4[0xa48 - 0x8c4];
    s32 type;
    u8 padA4C[0xa80 - 0xa4c];
    UnkStruct_0230d964_e *head;
    u8 padA84[4];
    u8 sentinel[4];
} UnkStruct_0230d964;

typedef struct UnkStruct_0230d964_t {
    s32 (*fn0)(void);
    void (*fn4)(void);
    void *unk8;
    void (*fnC)(UnkStruct_0230d964_m *m);
    void (*fn10)(UnkStruct_0230d964_m *m);
    void *unk14;
} UnkStruct_0230d964_t;

extern UnkStruct_0230d964 *gSpecialStageMap;
extern UnkStruct_0230d964_t gSpecialStageMapSegmentTypeTable[];
extern UnkStruct_0230d964_t data_ov001_02317228[];

BOOL SpecialStageMap_IsSegmentListEmpty(void);
void SpecialStageMap_FreeSegment(UnkStruct_0230d964_e *e);
void SpecialStageData_AddStep(void);
void MTX_Identity43_(UnkStruct_0230d964_m *m);
void Model_Animate(void *o);

void SpecialStageMap_Update(void) {
    UnkStruct_0230d964 *g = gSpecialStageMap;
    UnkStruct_0230d964_e *e;
    UnkStruct_0230d964_e *e2;
    UnkStruct_0230d964_e *e3;
    UnkStruct_0230d964_m m;
    s32 d;

    g->time += g->step;
    if (SpecialStageMap_IsSegmentListEmpty())
        return;
    for (e = g->head; e != (UnkStruct_0230d964_e *)g->sentinel; e = e->next) {
        if (e->flags & 1)
            SpecialStageMap_FreeSegment(e);
    }
    e2 = g->head;
    d = gSpecialStageMapSegmentTypeTable[e2->type].fn0();
    if (g->time > d) {
        g->type = e2->type;
        g->time -= d;
        e2->flags |= 1;
        SpecialStageData_AddStep();
    }
    if (SpecialStageMap_IsSegmentListEmpty())
        return;
    for (e2 = g->head; e2 != (UnkStruct_0230d964_e *)g->sentinel; e2 = e2->next) {
        if (!(e2->flags & 1))
            break;
    }
    MTX_Identity43_(&g->mtx);
    ((void (*)(void))data_ov001_02317228[e2->type].fn0)();
    m = g->mtx;
    for (e3 = g->head; e3 != (UnkStruct_0230d964_e *)g->sentinel; e3 = e3->next) {
        if (e3->flags & 1)
            continue;
        e3->mtx = m;
        gSpecialStageMapSegmentTypeTable[e3->type].fnC(&m);
        gSpecialStageMapSegmentTypeTable[e3->type].fn10(&m);
    }
    Model_Animate(g->obj8C4);
}
