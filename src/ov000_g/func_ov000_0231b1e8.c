#include <ov000_b/stageobj.h>

typedef struct {
    u8 unk0;
    u8 pad1[0x2E - 1];
    s16 unk2E;
    u8 pad30[0x34 - 0x30];
    s16 unk34;
    u8 pad36[0x62 - 0x36];
    s16 unk62;
    u8 pad64[0x37C - 0x64];
    s16 unk37C;
    u8 pad37E[2];
    s16 unk380;
} UnkStruct_0231b1e8_P;

typedef struct {
    u8 pad0[0x1E0];
    UnkStruct_ObjParam *unk1E0;
} UnkStruct_0231b1e8_O;

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkStruct_0231b1e8_C;

void func_ov000_0231b4b0(UnkStruct_0231b1e8_O *o, s16 v);
void ObjRect_FuncNoHit(UnkStruct_0231b1e8_C *a, UnkStruct_0231b1e8_C *b);

#define ABS(x) ((x) < 0 ? -(x) : (x))

static inline UnkStruct_0231b1e8_P *GetPlayer(UnkStruct_0231b1e8_C *c) {
    UnkStruct_0231b1e8_P *r = NULL;
    UnkStruct_0231b1e8_P *p = c->unk18;
    if (p != NULL) {
        r = p;
        if (p->unk0 != 1)
            r = NULL;
    }
    return r;
}

void func_ov000_0231b1e8(UnkStruct_0231b1e8_C *a, UnkStruct_0231b1e8_C *b) {
    UnkStruct_0231b1e8_O *o = a->unk18;
    UnkStruct_0231b1e8_P *p = GetPlayer(b);
    s16 s;
    s16 v;
    s32 mode;
    s16 h;

    if (p == NULL)
        return;
    v = p->unk34;
    s = ABS(v);
    h = p->unk2E;
    if (s < ABS(h)) {
        s = ABS(h);
        v = h;
    }
    mode = o->unk1E0->unk4 & 3;
    if (mode == 0 || (mode == 1 && s > p->unk37C) || (mode == 2 && s > p->unk380)) {
        func_ov000_0231b4b0(o, v);
        p->unk62 = 4;
    } else {
        ObjRect_FuncNoHit(a, b);
    }
}
