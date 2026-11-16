#include <ov000_c/stageobj.h>

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x224 - 0x20C];
    s32 unk224;
} UnkStruct_Boss;

typedef struct {
    UnkStruct_StageObj base;
    u8 pad20C[0x440 - 0x20C];
    void *unk440;
    u8 pad444[0x466 - 0x444];
    u16 unk466;
} UnkStruct_022e8050;

extern UnkStruct_Boss *data_ov000_02369b78;
extern UnkStruct_StageObj *data_ov000_0236ac60;

void func_ov000_022e8538(UnkStruct_022e8050 *w);
void func_ov000_022e7f60(void);

void func_ov000_022e8050(UnkStruct_022e8050 *w) {
    UnkStruct_Boss *b = data_ov000_02369b78;

    func_ov000_022e8538(w);
    if (b->unk224 < data_ov000_0236ac60->unk10) {
        if (w->unk466 < 0xaaab)
            return;
        if (w->unk466 <= 0xd555) {
            w->base.unk2E = 0xf00;
            w->unk440 = func_ov000_022e7f60;
        }
    } else {
        if (w->unk466 < 0x2aab)
            return;
        if (w->unk466 <= 0x5555) {
            w->base.unk2E = -0xf00;
            w->unk440 = func_ov000_022e7f60;
        }
    }
}
