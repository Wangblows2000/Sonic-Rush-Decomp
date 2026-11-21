#include <nitro/types.h>

typedef struct UnkStruct_02306d04_t {
    s16 unk0;
    u8 pad2[0x18 - 2];
} UnkStruct_02306d04_t;

typedef struct UnkStruct_02306d04_a {
    u8 pad0[0xbc];
    u32 flags;
} UnkStruct_02306d04_a;

typedef struct UnkStruct_02306d04_b {
    u8 pad0[0x10c];
    u8 unk10C;
} UnkStruct_02306d04_b;

typedef struct UnkStruct_02306d04 {
    u8 pad0[0x68];
    s16 type;
    u8 pad6A[0x70 - 0x6a];
    s32 unk70;
    u8 pad74[0xac - 0x74];
    UnkStruct_02306d04_b *unkAC;
    UnkStruct_02306d04_a *unkB0;
} UnkStruct_02306d04;

extern UnkStruct_02306d04_t data_ov001_02316ff6[];

void SpecialStageActObj_Destroy(UnkStruct_02306d04 *p);

void SpecialStage_ActObjDestroyWhenDone(UnkStruct_02306d04 *p) {
    if (data_ov001_02316ff6[p->type].unk0 <= 0) {
        if (p->unkB0 != NULL && (p->unkB0->flags & 2)) {
            SpecialStageActObj_Destroy(p);
            return;
        }
        if (p->unkAC == NULL)
            return;
        if (!(p->unkAC->unk10C & 2))
            return;
        SpecialStageActObj_Destroy(p);
        return;
    }
    if (p->unk70 > 1)
        return;
    SpecialStageActObj_Destroy(p);
}
