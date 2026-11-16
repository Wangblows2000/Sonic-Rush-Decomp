#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x3D4 - 0x20C];
    s32 unk3D4;
    u8 pad3D8[0x432 - 0x3D8];
    s16 unk432;
    s16 unk434;
    s16 unk436;
} UnkStruct_022e99e0;

void func_ov000_022ee198(UnkStruct_022e99e0 *s, s32 st);
void func_ov000_022e9b70(UnkStruct_022e99e0 *s, s32 a, s32 b);

void func_ov000_022e99e0(UnkStruct_022e99e0 *s) {
    u16 v;

    v = s->base.unk2E < 0 ? -s->base.unk2E : s->base.unk2E;
    if (s->unk3D4 < 0x3d || s->unk3D4 > 0x42)
        func_ov000_022ee198(s, 0x3d);
    if (s->unk3D4 == 0x3d && (s->base.unk5C & 8) && v >= s->unk432)
        func_ov000_022ee198(s, 0x3e);
    if (s->unk3D4 == 0x3e && (s->base.unk5C & 8)) {
        if (v >= s->unk434)
            func_ov000_022ee198(s, 0x3f);
        else
            func_ov000_022ee198(s, 0x3d);
    }
    if (s->unk3D4 == 0x3f && (s->base.unk5C & 8)) {
        if (v >= s->unk436)
            func_ov000_022ee198(s, 0x40);
        else
            func_ov000_022ee198(s, 0x3e);
    }
    s->base.unk5C |= 4;
    func_ov000_022e9b70(s, 1, 0);
}
