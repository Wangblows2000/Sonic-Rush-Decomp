#include <nitro/types.h>

typedef struct UnkStruct_022ffeb0_t {
    u8 pad0[8];
    void (*fn)(void);
} UnkStruct_022ffeb0_t;

typedef struct UnkStruct_022ffeb0 {
    u32 flags;
    s32 unk4;
    UnkStruct_022ffeb0_t *task8;
    UnkStruct_022ffeb0_t *taskC;
    UnkStruct_022ffeb0_t *task10;
    u8 pad14[0x40 - 0x14];
    s32 unk40;
    u8 pad44[0x50 - 0x44];
    s32 unk50;
    s32 unk54;
    s32 unk58;
    s32 unk5C;
} UnkStruct_022ffeb0;

typedef struct UnkStruct_022ffeb0_g {
    u8 pad0[0x40];
    s32 unk40;
} UnkStruct_022ffeb0_g;

extern UnkStruct_022ffeb0 *data_ov001_0231f52c;
extern u8 data_02074ba4[];

UnkStruct_022ffeb0_g *func_ov001_022ffce8(void);
void func_0201c070(void *pp, s32 a);
s32 func_0200203c(s32 n, s32 d);
void func_ov001_022ff008(void);
void func_ov001_022fef3c(void);
void func_ov001_022feee4(void);
void func_ov001_02302a3c(void);
void func_ov001_02304fb4(void);
void func_ov001_02304460(void);
void func_ov001_022fd4d4(void);
void func_ov001_0230582c(void);
void func_ov001_023057f4(void);
void func_ov001_022ee964(void);
void func_ov001_02306424(void);
void func_02004084(const void *src);

static inline BOOL IsMulti_022ffeb0(void) {
    if (func_ov001_022ffce8()->unk40 > 1)
        return TRUE;
    return FALSE;
}

void func_ov001_022ffeb0(void) {
    func_0201c070((u8 *)func_ov001_022ffce8() + 0x33c, 0);
    if (func_0200203c(data_ov001_0231f52c->unk40, 2) != 0)
        *(vu16 *)0x4000304 &= ~0x8000;
    else
        *(vu16 *)0x4000304 |= 0x8000;
    data_ov001_0231f52c->task8->fn = func_ov001_022ff008;
    data_ov001_0231f52c->taskC->fn = func_ov001_022fef3c;
    data_ov001_0231f52c->task10->fn = func_ov001_022feee4;
    data_ov001_0231f52c->flags &= ~0x72;
    data_ov001_0231f52c->flags |= 4;
    func_ov001_02302a3c();
    func_ov001_02304fb4();
    func_ov001_02304460();
    func_ov001_022fd4d4();
    if (IsMulti_022ffeb0()) {
        data_ov001_0231f52c->unk50 = 0;
        data_ov001_0231f52c->unk54 = 0;
        data_ov001_0231f52c->unk58 = 0x180000;
        data_ov001_0231f52c->unk5C = 0x180000;
    } else {
        data_ov001_0231f52c->unk50 = 0x50000;
        data_ov001_0231f52c->unk54 = 0;
        data_ov001_0231f52c->unk58 = 0x100000;
        data_ov001_0231f52c->unk5C = 0x130000;
    }
    func_ov001_0230582c();
    func_ov001_023057f4();
    if (IsMulti_022ffeb0())
        func_ov001_022ee964();
    *(vu16 *)0x4000008 = *(vu16 *)0x4000008 & ~3;
    *(vu16 *)0x400000c = (*(vu16 *)0x400000c & ~3) | 2;
    *(vu32 *)0x4000000 = (*(vu32 *)0x4000000 & ~0x1f00) | 0x1300;
    func_ov001_02306424();
    func_02004084(data_02074ba4);
}
