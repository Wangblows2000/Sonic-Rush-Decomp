#include <nitro/types.h>

typedef struct {
    s32 id;
    u8 pad4[0x2c];
} UnkItem_022ce164;

typedef struct {
    u8 pad0[4];
    s32 unk4;
    s32 unk8;
    s32 count;
    s32 unk10;
    u8 pad14[8];
    s32 unk1c;
    u8 pad20[8];
    s32 unk28;
    u8 pad2c[4];
    s32 unk30;
    s32 unk34;
    u8 pad38[0x1b8 - 0x38];
    s32 unk1b8;
    u8 pad1bc[0x28c - 0x1bc];
    UnkItem_022ce164 items[6];
    void *parent;
} UnkWork_022ce164;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022ce164 *work;
} UnkTask_022ce164;

typedef struct {
    u8 pad0[0x18];
    void *ptrs[6];
    u8 pad30[0x1e04 - 0x30];
    s32 unk1e04;
} UnkStruct_022ce164;

UnkTask_022ce164 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void func_ov001_022cdbc4(void);
void func_ov001_022cc044(UnkWork_022ce164 *w);
void func_ov001_022cbc2c(UnkWork_022ce164 *w);

#pragma thumb on
void func_ov001_022ce164(UnkStruct_022ce164 *p) {
    UnkWork_022ce164 *w;
    vu16 zero;
    s32 n;
    s32 i;

    w = Task_Create(func_ov001_022cdbc4, NULL, 0, 0x2000, 0x1000, sizeof(UnkWork_022ce164))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_022ce164));
    w->parent = p;
    n = 0;
    w->unk4 = 0;
    w->unk8 = -1;
    w->unk1c = -1;
    w->unk30 = -1;
    w->unk34 = -1;
    w->unk1b8 = p->unk1e04;
    w->unk10 = 0;
    w->unk28 = -1;
    for (i = 0; i < 6; i++) {
        if (p->ptrs[i] != NULL) {
            w->items[n].id = i;
            n++;
        }
    }
    w->count = n;
    for (; n < 6; n++) {
        w->items[n].id = 7;
    }
    func_ov001_022cc044(w);
    func_ov001_022cbc2c(w);
}
#pragma thumb reset
