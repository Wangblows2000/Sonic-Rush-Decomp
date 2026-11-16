#include <nitro/types.h>

void func_02021dc4(void *bits, void *res);
void addLink_(void *p, void *res);

typedef struct { u8 unk0[8]; const u8 *type; } UnkRes_02021d28;
typedef struct { u8 unk0[8]; u8 unk8[8]; u8 unk10[8]; u8 unk18[8]; u8 unk20[0x1C]; u8 unk3C[8]; u8 unk44[8]; u8 unk4C[8]; } UnkStruct_02021d28;

void NNS_G3dRenderObjAddAnmObj(UnkStruct_02021d28 *s, UnkRes_02021d28 *r) {
    switch (*r->type) {
    case 'M':
        func_02021dc4(s->unk3C, r);
        addLink_(s->unk8, r);
        break;
    case 'J':
        func_02021dc4(s->unk44, r);
        addLink_(s->unk10, r);
        break;
    case 'V':
        func_02021dc4(s->unk4C, r);
        addLink_(s->unk18, r);
        break;
    }
}
