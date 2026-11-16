#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x34 - 1];
    s16 unk34;
    u8 pad36[0x58 - 0x36];
    u32 unk58;
    u8 pad5C[0x328 - 0x5C];
    s16 unk328;
} UnkStruct_0231cda4_P;

typedef struct {
    u8 pad0[0x1E0];
    UnkStruct_ObjParam *unk1E0;
} UnkStruct_0231cda4_O;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_0231cda4_C;

void func_ov000_02353820(UnkStruct_0231cda4_P *p, UnkStruct_0231cda4_O *o);

void MapObj80_OnHit(UnkStruct_0231cda4_C *a, UnkStruct_0231cda4_C *b) {
    UnkStruct_0231cda4_O *o = a->unk18;
    UnkStruct_0231cda4_P *p = b->unk18;

    if (p == NULL)
        return;
    if (p->unk0 != 1)
        return;
    if (p->unk328 != 0x27) {
        if (!(p->unk58 & 1))
            return;
        if ((p->unk34 >= 0 && !(o->unk1E0->unk4 & 1)) || (p->unk34 < 0 && (o->unk1E0->unk4 & 1)))
            func_ov000_02353820(p, o);
    } else {
        if (!(o->unk1E0->unk4 & 2))
            return;
        func_ov000_02353820(p, o);
    }
}
