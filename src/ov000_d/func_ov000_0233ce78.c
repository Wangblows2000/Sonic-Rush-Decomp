#include <nitro/types.h>

typedef struct {
    u8 pad0[0x84];
    void *unk84;
    void *unk88;
} UnkStruct_0233ce78_Vram;

typedef struct {
    u8 pad0[0x2AC];
    u8 unk2AC[0x6AC - 0x2AC];
    u8 unk6AC[0x8B0 - 0x6AC];
    u8 *unk8B0;
} UnkStruct_0233ce78;

extern UnkStruct_0233ce78_Vram data_0208922c;
extern UnkStruct_0233ce78_Vram data_02087d04;

void DC_StoreRange(void *p, u32 size);
void DC_WaitWriteBufferEmpty(void);
void func_02007d2c(u32 ch, const void *src, void *dst, u32 size);
s32 func_0200311c(void);
void func_02003788(s32 a);
s32 func_02003080(void);
void func_02003204(s32 a);

void func_ov000_0233ce78(UnkStruct_0233ce78 *w, s32 sub) {
    s32 st;
    u8 *p;

    DC_StoreRange(w, 0x950);
    DC_WaitWriteBufferEmpty();
    if (sub == 0) {
        func_02007d2c(3, w->unk6AC, (void *)0x5000000, 0x1e0);
        st = func_0200311c();
        p = w->unk2AC;
        func_02007d2c(3, p, data_0208922c.unk84, 0x1e0);
        func_02007d2c(3, p, data_0208922c.unk88, 0x1e0);
        func_02003788(st);
        func_02007d2c(3, w->unk8B0, (void *)0x5000200, 0);
        func_02007d2c(3, w->unk8B0 + 0x40, (void *)0x5000240, 0x1c0);
    } else {
        func_02007d2c(3, w->unk6AC, (void *)0x5000400, 0x1e0);
        st = func_02003080();
        p = w->unk2AC;
        func_02007d2c(3, p, data_02087d04.unk84, 0x1e0);
        func_02007d2c(3, p, data_02087d04.unk88, 0x1e0);
        func_02003204(st);
        func_02007d2c(3, w->unk8B0, (void *)0x5000600, 0);
        func_02007d2c(3, w->unk8B0 + 0x40, (void *)0x5000640, 0x1c0);
    }
}
