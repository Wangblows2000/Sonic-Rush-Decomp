#include <nitro/types.h>

typedef struct {
    u8 pad0[0x325];
    u8 unk325;
    u8 pad326[0x32C - 0x326];
    u32 unk32C;
} UnkStruct_02345e98;

typedef struct {
    u8 pad0[0x10];
    u32 unk10;
    u8 pad14[0x18 - 0x14];
    u32 unk18;
    u8 pad1C[0x20 - 0x1C];
    u32 unk20;
} UnkStruct_02345e98_4560;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
    u8 pad1C[0x24 - 0x1C];
    u32 unk24;
    u8 pad28[0x40 - 0x28];
    u32 unk40;
} UnkStruct_02345e98_4590;

typedef struct {
    u8 pad0[8];
    u32 unk8;
} UnkStruct_02345e98_4680;

extern UnkStruct_02345e98_4560 data_022c4560;
extern UnkStruct_02345e98_4590 data_022c4590;
extern UnkStruct_02345e98_4680 data_022c4680;

void func_02043c7c(s32 a, s32 b);
void func_ov000_02347c6c(UnkStruct_02345e98 *w, s32 a);
void func_ov000_02349038(UnkStruct_02345e98 *w);
void func_0204b7ec(void *p, s32 a, s32 b);

void func_ov000_02345e98(UnkStruct_02345e98 *w) {
    if (w->unk325 != 0) {
        return;
    }
    if (data_022c4560.unk10 == 1) {
        if (data_022c4560.unk20 >= 0x8c9f && !(w->unk32C & 0x1000000)) {
            func_02043c7c(4, 0x80);
            func_ov000_02347c6c(w, 0);
            data_022c4590.unk40 = 3;
        }
    } else if (!(data_022c4680.unk8 & 2) || data_022c4560.unk10 == 2) {
        if (data_022c4560.unk20 >= 0x8c9f) {
            data_022c4590.unk18 = 0;
            data_022c4590.unk24 |= 4;
            func_ov000_02349038(w);
        }
    }
    if (!(data_022c4560.unk18 & 1)) {
        return;
    }
    if (data_022c4560.unk20 < 0x87ef) {
        return;
    }
    if (data_022c4560.unk20 == 0x87ef || data_022c4560.unk20 == 0x8867 || data_022c4560.unk20 == 0x88df ||
        data_022c4560.unk20 == 0x8957 || data_022c4560.unk20 == 0x89cf || data_022c4560.unk20 == 0x8a47 ||
        data_022c4560.unk20 == 0x8abf || data_022c4560.unk20 == 0x8b37 || data_022c4560.unk20 == 0x8baf ||
        data_022c4560.unk20 == 0x8c27) {
        func_0204b7ec(NULL, 0, 0x36);
    }
}
