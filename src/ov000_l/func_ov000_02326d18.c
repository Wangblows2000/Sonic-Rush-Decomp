#include <nitro/types.h>

typedef struct UnkNode_02326d18 {
    u8 pad0[0x18];
    struct UnkNode_02326d18 *next;
} UnkNode_02326d18;

typedef struct {
    u8 unk0;
    u8 pad1[0x32b];
    u32 unk32C;
} UnkObj_02326d18;

typedef struct {
    u8 pad0[0x54];
    u32 unk54;
    u8 pad58[4];
    u16 unk5C;
    u8 pad5E[0x70 - 0x5e];
    void *unk70;
    u8 pad74[0x134 - 0x74];
    UnkObj_02326d18 *unk134;
    void *unk138;
    u8 pad13C[0x29c - 0x13c];
    UnkNode_02326d18 *unk29C;
    s32 unk2A0;
} UnkWork_02326d18;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02326d18 *work;
} UnkTask_02326d18;

extern UnkTask_02326d18 *gTaskCurrent;

void func_ov000_02326ac8(UnkWork_02326d18 *w);
void func_ov000_02326b2c(UnkNode_02326d18 *n, UnkObj_02326d18 *o, void *a);

void func_ov000_02326d18(void) {
    UnkWork_02326d18 *w = gTaskCurrent->work;
    UnkObj_02326d18 *o = w->unk134;
    UnkNode_02326d18 *n;

    if (o->unk0 == 1 && (o->unk32C & 0x80)) {
        w->unk5C |= 0x20;
        w->unk70 = NULL;
        w->unk54 |= 4;
        return;
    }
    n = w->unk29C->next;
    w->unk29C = n;
    func_ov000_02326ac8(w);
    func_ov000_02326b2c(n, o, w->unk138);
    w->unk2A0--;
    if (w->unk2A0 <= 0) {
        w->unk5C |= 0x20;
        w->unk70 = NULL;
        w->unk54 |= 4;
    }
}
