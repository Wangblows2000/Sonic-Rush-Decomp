#include <nitro/types.h>

typedef struct {
    u8 unk0;
    u8 unk1;
} UnkStruct_022fb9c8_P;

typedef struct {
    u8 pad0[0x1DC];
    s32 unk1DC;
    UnkStruct_022fb9c8_P *unk1E0;
    u8 unk1E4;
    u8 pad1E5[0x1F4 - 0x1E5];
    u32 unk1F4;
} UnkStruct_022fb9c8_W;

typedef struct {
    u8 pad[0x10];
    UnkStruct_022fb9c8_W *unk10;
} UnkStruct_022fb9c8;

void func_0201bf1c(s32 a);
void func_020365bc(s32 a);
void func_ov000_022fae34(UnkStruct_022fb9c8_P *p);
void func_0206bef4(UnkStruct_022fb9c8 *t);

void func_ov000_022fb9c8(UnkStruct_022fb9c8 *t) {
    UnkStruct_022fb9c8_W *w = t->unk10;

    if (w->unk1DC != 0)
        func_0201bf1c(w->unk1DC);
    if (w->unk1DC != 0)
        func_020365bc(w->unk1DC);
    if (w->unk1E0 != NULL && w->unk1E0->unk0 == 0xff && w->unk1E0->unk1 == 0xff)
        func_ov000_022fae34(w->unk1E0);
    else if (!(w->unk1F4 & 0x10) && w->unk1E0 != NULL)
        w->unk1E0->unk0 = w->unk1E4;
    func_0206bef4(t);
}
