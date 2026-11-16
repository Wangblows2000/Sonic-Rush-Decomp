#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[0x10 - 4];
    s16 unk10;
    u8 pad12[0x18 - 0x12];
    u8 *unk18;
} UnkEnt_0235c168;

typedef struct {
    u8 pad0[0x48];
    void *unk48;
    u8 pad4C[0x90 - 0x4c];
    void *unk90;
    u8 pad94[0xa4 - 0x94];
    u8 unkA4[0x198 - 0xa4];
    void *unk198;
    void *unk19C;
    UnkEnt_0235c168 e[64];
} UnkWork_0235c168;

typedef struct {
    u8 pad0[0x10];
    UnkWork_0235c168 *unk10;
} UnkTask_0235c168;

typedef struct {
    u8 pad0[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkCfg_0235c168;

extern UnkCfg_0235c168 data_022c4560;
extern void *gRingManagerTask;
void ObjVram_Free(s32 a, void *p);
void Model_Release(void *p);

void RingManager_Destructor(UnkTask_0235c168 *t) {
    UnkWork_0235c168 *w = t->unk10;
    u32 i;
    UnkEnt_0235c168 *e;

    e = w->e;
    for (i = 0; i < 64; i++) {
        if (e->unk10 != 0 && e->unk18 != NULL)
            *e->unk18 = e->unk0 >> 8;
        e++;
    }
    if (w->unk48 != NULL)
        ObjVram_Free(0, w->unk48);
    if (w->unk90 != NULL)
        ObjVram_Free(1, w->unk90);
    if (data_022c4560.unk1C >= 9 || data_022c4560.unk1D >= 2)
        Model_Release(w->unkA4);
    if (w->unk198 != NULL)
        ObjVram_Free(0, w->unk198);
    if (w->unk19C != NULL)
        ObjVram_Free(1, w->unk19C);
    gRingManagerTask = NULL;
}
