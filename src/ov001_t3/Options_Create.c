#include <nitro/types.h>

typedef struct {
    s32 unk0;
    s32 unk4;
    u8 unk8[0x14 - 8];
    s32 unk14;
    s32 unk18;
    s32 unk1c;
    s32 unk20;
    s32 unk24;
    s32 unk28;
    s32 unk2c;
    u8 pad30[0x1e04 - 0x30];
    u8 *buf;
    u8 pad1e08[0x1e48 - 0x1e08];
} UnkWork_022cecb8;

typedef struct {
    u8 pad0[0x10];
    UnkWork_022cecb8 *work;
} UnkTask_022cecb8;

extern void *gMainHeap;
extern u8 gPad[];

UnkTask_022cecb8 *Task_Create(void (*fn)(void), void (*dtor)(void), s32 b, s32 c, s32 d, s32 size);
void MIi_CpuClear16(u16 data, void *dest, u32 size);
void Options_Main(void);
void Options_Destructor(void);
void func_ov001_022cb6d8(void *p);
s32 func_ov001_022c8b5c(void);
s32 func_ov001_022c8b58(void);
s32 func_ov001_022c8b54(void);
s32 func_ov001_022c8b38(void);
s32 func_ov001_022c8b34(void);
s32 func_ov001_022c8b30(void);
s32 func_ov001_022c8b14(void);
u8 *Heap_Alloc(void *heap, u32 size, s32 align);
void *OS_GetArenaLo(s32 i);
void MIi_CpuClear32(u32 data, void *dest, u32 size);
void DC_StoreRange(void *p, u32 size);
void func_ov001_022cea0c(void);
void Options_LoadAssets(UnkWork_022cecb8 *w);
void func_ov001_022ce960(UnkWork_022cecb8 *w);
void func_ov001_022ce6f8(UnkWork_022cecb8 *w);
void func_ov001_022ce4cc(UnkWork_022cecb8 *w);
void Pad_Reset(void *p, s32 a, s32 b);
void Touch_Enable(void);
void func_ov001_022c8994(void);
void SysSound_Init(void);
void func_02052168(s32 a);

#pragma thumb on
static inline u8 *Alloc_022cecb8(u32 size) {
    u8 *blk = Heap_Alloc(gMainHeap, size, 0x20);
    if ((u8 *)OS_GetArenaLo(0) != blk) {
        vu32 fill = 0xcccccccc;
        MIi_CpuClear32(fill, blk, size);
        *(u32 *)blk = size;
        *(u32 *)(blk + size - 4) = 0x02467531;
        DC_StoreRange(blk, size);
        blk += 0x20;
    }
    return blk;
}

UnkWork_022cecb8 *Options_Create(void) {
    UnkWork_022cecb8 *w;
    vu16 zero;

    w = Task_Create(Options_Main, Options_Destructor, 0, 0x1000, 0x1000, sizeof(UnkWork_022cecb8))->work;
    zero = 0;
    MIi_CpuClear16(zero, w, sizeof(UnkWork_022cecb8));
    func_ov001_022cb6d8(w->unk8);
    w->unk0 = 0;
    w->unk4 = 6;
    w->unk18 = func_ov001_022c8b5c();
    w->unk1c = func_ov001_022c8b58();
    w->unk20 = func_ov001_022c8b54();
    w->unk24 = func_ov001_022c8b38();
    w->unk28 = func_ov001_022c8b34();
    w->unk2c = func_ov001_022c8b30();
    w->unk14 = func_ov001_022c8b14();
    w->buf = Alloc_022cecb8(0xc0);
    func_ov001_022cea0c();
    Options_LoadAssets(w);
    func_ov001_022ce960(w);
    func_ov001_022ce6f8(w);
    func_ov001_022ce4cc(w);
    Pad_Reset(gPad, 0, 0);
    Touch_Enable();
    func_ov001_022c8994();
    SysSound_Init();
    func_02052168(0);
    return w;
}
#pragma thumb reset
