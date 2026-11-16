#include <ov000_b/stageobj.h>

typedef struct {
    u8 pad0[2];
    u16 unk2;
} UnkStruct_0231dee0_A;

typedef struct {
    UnkStruct_EffObj base;
    u8 pad134[0x184 - 0x134];
    UnkStruct_0231dee0_A *unk184;
    u8 pad188[0x194 - 0x188];
    u32 unk194;
} UnkStruct_0231dee0;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_022c4560;

extern UnkStruct_022c4560 data_022c4560;

s16 ObjSpd_DownSet(s32 v, s32 d);
void func_ov000_0231e06c(UnkStruct_0231dee0 *w);
void func_ov000_0231d904(UnkStruct_0231dee0 *w);
void func_ov000_0231db90(UnkStruct_0231dee0 *w);

void func_ov000_0231dee0(UnkStruct_0231dee0 *w) {
    if (w->base.unk58 & 1) {
        if ((w->base.unk58 & 0x10) && w->unk184->unk2 == 10) {
            StageObj_SetAnimation(w, 0);
            w->base.unk5C |= 4;
        }
        w->base.unk58 &= ~0x10;
        if (w->unk194 & 0x30) {
            if (w->base.unkC == 0) {
                if (!(data_022c4560.unk20 & 7)) {
                    func_ov000_0231e06c(w);
                    return;
                }
            } else {
                w->base.unkC--;
            }
        }
    }
    w->base.unk2E = ObjSpd_DownSet(w->base.unk2E, 0x28);
    func_ov000_0231d904(w);
    func_ov000_0231db90(w);
}
