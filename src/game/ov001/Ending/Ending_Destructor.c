#include <nitro/types.h>

typedef struct UnkStruct_022c8404_e {
    u8 pad0[0x48];
} UnkStruct_022c8404_e;

typedef struct UnkStruct_022c8404_w {
    u8 pad0[0x558];
    u32 unk558;
} UnkStruct_022c8404_w;

typedef struct UnkStruct_022c8404_t {
    u8 pad0[0x10];
    UnkStruct_022c8404_e *unk10;
} UnkStruct_022c8404_t;

extern UnkStruct_022c8404_t *gTaskCurrent;
void ObjVram_Free(s32 a, u32 b);
void Ending_ReleaseAssets(void *w);

void Ending_Destructor(void) {
    UnkStruct_022c8404_e *w;
    s32 i;
    w = gTaskCurrent->unk10;
    for (i = 0; i < 5; i++) {
        ObjVram_Free(1, ((UnkStruct_022c8404_w *)&w[i])->unk558);
    }
    Ending_ReleaseAssets(w);
}
