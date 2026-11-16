#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[0x18];
    void *unk18;
} UnkColl_0231bd44;

typedef struct {
    UnkStruct_StageObj base;
    u32 unk20C;
} UnkWork_0231bd44;

void func_ov000_02355b20(void *p, s32 vx, s32 vy);
void func_ov000_02355da0(void *p, UnkWork_0231bd44 *o);

void func_ov000_0231bd44(UnkColl_0231bd44 *a, UnkColl_0231bd44 *b) {
    UnkWork_0231bd44 *o = a->unk18;
    void *p = b->unk18;
    s16 vx = 0;
    s16 vy = 0;
    s16 v;
    u16 id;

    if (o == NULL)
        return;
    if (p == NULL)
        return;
    if (!(o->unk20C & 1))
        return;
    v = -0x780;
    v -= o->base.unk1E0->unk6 * 0x180;
    id = o->base.unk1E0->unk2;
    if (id == 0x85)
        vy = v;
    else if (id == 0x86)
        vx = v;
    else
        vx = -v;
    func_ov000_02355b20(p, vx, vy);
    func_ov000_02355da0(p, o);
}
