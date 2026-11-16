#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u16 unk2;
} UnkStruct_02301488_1e0;

typedef struct {
    u8 pad0[0x1e0];
    UnkStruct_02301488_1e0 *unk1E0;
    u8 pad1E4[0x204 - 0x1e4];
    void *unk204;
} UnkStruct_02301488_o;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_02301488;

void func_ov000_0232eaac(UnkStruct_02301488_o *o, s32 a, s32 b);
void func_ov000_0230169c(UnkStruct_02301488_o *o);

void func_ov000_02301488(UnkStruct_02301488 *a, UnkStruct_02301488 *b) {
    UnkStruct_02301488_o *o;
    void *t;

    t = b->unk18;
    o = a->unk18;

    if (t == NULL)
        return;
    if (o->unk1E0->unk2 != 5)
        return;
    func_ov000_0232eaac(o, 0, -0x20);
    o->unk204 = t;
    func_ov000_0230169c(o);
}
