#include <nitro/types.h>

typedef struct UnkStruct_022fb7b0 {
    u8 pad0[0x14];
    u32 flags;
    u8 pad18[0x74 - 0x18];
    void (*state)(struct UnkStruct_022fb7b0 *p);
    u8 pad78[4];
    struct UnkStruct_022fb7b0 *parent;
    u8 pad80[0x1cc - 0x80];
    s32 timer;
} UnkStruct_022fb7b0;

void ExtraBossObj_State_Unk_ov001_022fba78(UnkStruct_022fb7b0 *p);

void ExtraBossObj_State_Unk_ov001_022fb7b0(UnkStruct_022fb7b0 *p) {
    p->timer--;
    if (p->timer <= 0 || p->parent->state == ExtraBossObj_State_Unk_ov001_022fba78)
        p->flags |= 8;
}
