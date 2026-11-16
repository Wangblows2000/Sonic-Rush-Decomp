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
    u8 pad5E[0x68 - 0x5e];
    void *unk68;
    u8 pad6C[4];
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

void func_ov000_02326d18(void);

void func_ov000_02326dd8(void) {
    UnkWork_02326d18 *w = gTaskCurrent->work;
    UnkObj_02326d18 *o = w->unk134;
    UnkNode_02326d18 *n;

    if (o->unk0 == 1 && !(o->unk32C & 0x80)) {
        w->unk68 = func_ov000_02326d18;
        func_ov000_02326d18();
        return;
    }
    n = w->unk29C->next;
    w->unk29C = n;
    func_ov000_02326ac8(w);
    func_ov000_02326b2c(n, o, w->unk138);
}
