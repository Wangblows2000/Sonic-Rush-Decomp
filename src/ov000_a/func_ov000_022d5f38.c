#include <nitro/types.h>

typedef struct UnkStruct_022d5f38_h {
    s32 unk0;
    u8 pad[0x18 - 4];
    void *unk18;
    u8 pad1[0x2c - 0x1c];
} UnkStruct_022d5f38_h;

typedef struct UnkStruct_022d5f38_s {
    UnkStruct_022d5f38_h unk0[2];
} UnkStruct_022d5f38_s;

typedef struct UnkStruct_022d5f38 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x134 - 0x58];
    UnkStruct_022d5f38_h unk134[2];
    u8 pad2[0x1b8 - 0x18c];
    s32 unk1B8;
    u8 pad3[0x238 - 0x1bc];
    UnkStruct_022d5f38_h unk238[2];
} UnkStruct_022d5f38;

typedef struct UnkStruct_022d5f38_w {
    u8 pad[0x238];
    UnkStruct_022d5f38_h unk238[2];
} UnkStruct_022d5f38_w;

typedef struct UnkTask_022d5f38 {
    u8 pad[0x10];
    UnkStruct_022d5f38 *unk10;
} UnkTask_022d5f38;

extern UnkTask_022d5f38 *gTaskCurrent;

void StageObj_HandleCollider(UnkStruct_022d5f38 *o, UnkStruct_022d5f38_h *h);
void func_02067d58(s32 *p);

void func_ov000_022d5f38(void) {
    UnkStruct_022d5f38 *o = gTaskCurrent->unk10;
    UnkStruct_022d5f38_w *w = (UnkStruct_022d5f38_w *)o;
    s32 i;
    if (o->unk54 & 0xc)
        return;
    for (i = 0; i < 2; i++) {
        if (o->unk134[i].unk18 != NULL) {
            StageObj_HandleCollider(o, &o->unk134[i]);
            w->unk238[i] = o->unk134[i];
            StageObj_HandleCollider(o, &w->unk238[i]);
            if (w->unk238[i].unk0 + 0x40 < 0x2ff)
                w->unk238[i].unk0 += 0x5fe;
            else
                w->unk238[i].unk0 -= 0x5fe;
        }
    }
    if (o->unk1B8 != 0)
        func_02067d58(&o->unk1B8);
}
