#include <nitro/types.h>

typedef struct UnkStruct_022db894 {
    s32 unk0;
    u8 pad[0x18 - 4];
    void *unk18;
    u8 pad1[0x2c - 0x1c];
} UnkStruct_022db894;

void StageObj_HandleCollider(void *o, UnkStruct_022db894 *h);

void Boss4_Unk_ov000_022db894(UnkStruct_022db894 *h, UnkStruct_022db894 *out) {
    void *o = h->unk18;
    *out = *h;
    StageObj_HandleCollider(o, out);
    if (out->unk0 < 0x3a4)
        out->unk0 += 0x749;
    else
        out->unk0 -= 0x749;
}
