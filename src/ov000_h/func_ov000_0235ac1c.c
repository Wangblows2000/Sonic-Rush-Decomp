#include <nitro/types.h>

typedef struct {
    u8 pad0[0x3E];
    u16 unk3E;
    u32 unk40;
} UnkStruct_0235ac1c_Hdr;

typedef struct {
    u8 pad0[0x18];
    u32 hdrOfs;
} UnkStruct_0235ac1c_Res;

typedef struct {
    u8 pad0[0xA];
    u8 unkA;
    u8 unkB;
    u8 padC[0x18 - 0xC];
    s32 unk18;
    s32 unk1C;
    s32 unk20;
    u8 pad24[0x48 - 0x24];
    s32 unk48;
    s32 unk4C;
    s32 unk50;
    u8 pad54[0xF4 - 0x54];
} UnkStruct_0235ac1c;

typedef struct {
    u8 pad0[0x10];
    UnkStruct_0235ac1c *work;
} UnkStruct_0235ac1c_Task;

UnkStruct_0235ac1c_Task *Task_Create(void (*f)(void), void (*g)(void), s32 b, s32 c, s32 d, s32 e);
void *func_02006abc(u32 arg);
void MIi_CpuClear16(u16 data, void *dst, u32 size);
s32 VramTex_Alloc(u32 a, s32 b);
s32 VramPltt_Alloc(u16 a, s32 b);
void Model_InitSprite3D(void *s, void *res, s32 a, s32 b, s32 c, s32 d, s32 e, s32 f);
void func_ov000_0235abd8(void);
void func_ov000_0235abc8(void);

void func_ov000_0235ac1c(s32 a, s32 b, s32 c, UnkStruct_0235ac1c_Res *res) {
    UnkStruct_0235ac1c_Task *t;
    UnkStruct_0235ac1c *w;
    UnkStruct_0235ac1c_Hdr *hdr;
    vu16 zero;

    t = Task_Create(func_ov000_0235abd8, func_ov000_0235abc8, 0, 0x3000, 2, sizeof(UnkStruct_0235ac1c));
    if (t == func_02006abc(0))
        return;
    w = t->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkStruct_0235ac1c));
    hdr = (UnkStruct_0235ac1c_Hdr *)((u8 *)res + res->hdrOfs);
    Model_InitSprite3D(w, res, 1, 0, VramTex_Alloc(hdr->unk40, 0), VramPltt_Alloc(hdr->unk3E, 0), 0, 0);
    w->unk48 = a;
    w->unk4C = b;
    w->unk50 = c;
    w->unk18 = w->unk1C = w->unk20 = 0x800;
    w->unkA = 0x13;
    w->unkB = 7;
}
