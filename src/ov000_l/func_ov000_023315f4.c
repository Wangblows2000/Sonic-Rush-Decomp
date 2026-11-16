#include <ov000_b/stageobj.h>

UnkStruct_EffObj *func_ov000_02331670(s32 a, s32 b, s32 c);
void func_ov000_022c5e80(s32 a, s32 b, s32 x, s32 *ox, s32 *oy, s32 *oz);
void func_ov000_0232e844(UnkStruct_EffObj *w);

UnkStruct_EffObj *func_ov000_023315f4(s32 a, s32 b, s32 c, s32 e0, s32 e4) {
    UnkStruct_EffObj *w;
    s32 z;
    s32 x;
    s32 y;

    w = func_ov000_02331670(a, b, c);
    if (w == NULL)
        return NULL;
    func_ov000_022c5e80(e0, e4, w->unk10 << 4, &x, &y, &z);
    w->unk10 = x >> 4;
    w->unk18 = y >> 4;
    func_ov000_0232e844(w);
    return w;
}
