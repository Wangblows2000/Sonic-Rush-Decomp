#include <ov000_b/stageobj.h>

typedef struct UnkStruct_02326f48_Node {
    u8 pad0[0x18];
    struct UnkStruct_02326f48_Node *next;
    struct UnkStruct_02326f48_Node *prev;
} UnkStruct_02326f48_Node;

typedef struct {
    u8 unk0;
    u8 pad1[0x54 - 1];
    u32 unk54;
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void *unk68;
    u8 pad6C[0x88 - 0x6C];
    void *unk88;
    u8 pad8C[0x134 - 0x8C];
    void *unk134;
    s32 unk138;
    UnkStruct_02326f48_Node nodes[11];
    UnkStruct_02326f48_Node *unk29C;
    u8 pad2A0[4];
    s32 unk2A4;
    s16 unk2A8;
    u16 unk2AA;
    s32 unk2AC;
    s32 unk2B0;
    s32 unk2B4;
    s32 unk2B8;
    s32 unk2BC;
} UnkStruct_02326f48;

extern u8 data_ov000_0236a970;

void func_ov000_02326e4c(void);
void func_ov000_02326ac8(UnkStruct_02326f48 *w);
void func_ov000_02326b2c(UnkStruct_02326f48_Node *n, void *a, s32 b);

UnkStruct_Task *func_ov000_02326f48(void *arg, s32 a, s32 b, s16 c, u16 d) {
    UnkStruct_Task *task;
    UnkStruct_02326f48 *w;
    vu16 zero;
    s32 i;

    data_ov000_0236a970 = 0;
    task = Task_Create(StageObj_Main, StageObj_Destructor, 0, 0xa00, 4, sizeof(UnkStruct_02326f48));
    if (task == func_02006abc(0))
        return NULL;
    w = task->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_02326f48));
    w->unk134 = arg;
    w->unk138 = a << 8;
    w->unk2A4 = b;
    for (i = 0; i < 10; i++)
        w->nodes[i].next = &w->nodes[i + 1];
    w->nodes[10].next = &w->nodes[0];
    for (i = 10; i > 0; i--)
        w->nodes[i].prev = &w->nodes[i - 1];
    w->nodes[0].prev = &w->nodes[10];
    w->unk29C = &w->nodes[0];
    func_ov000_02326ac8(w);
    func_ov000_02326b2c(&w->nodes[0], arg, w->unk138);
    w->unk2A8 = c;
    w->unk2AA = d;
    w->unk2AC = 6;
    w->unk2B0 = 0x12;
    w->unk2B4 = 1;
    w->unk2B8 = 0xa;
    w->unk2BC = 3;
    w->unk0 = 5;
    w->unk54 |= 0x12;
    w->unk58 |= 0x2300;
    w->unk5C |= 0x20;
    w->unk88 = arg;
    w->unk68 = func_ov000_02326e4c;
    return task;
}
