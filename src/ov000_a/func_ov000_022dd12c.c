#include <nitro/types.h>

typedef struct UnkStruct_022dd12c_w {
    u8 pad[0x224];
    s32 unk224;
} UnkStruct_022dd12c_w;

typedef struct UnkStruct_022dd12c_p {
    u8 pad[0x5c];
    u16 unk5C;
    u8 pad1[0x3ae - 0x5e];
    s16 unk3AE;
    u8 pad2[0x3b2 - 0x3b0];
    s16 unk3B2;
} UnkStruct_022dd12c_p;

typedef struct UnkStruct_022dd12c_g {
    u8 pad[0x48];
    s16 unk48;
} UnkStruct_022dd12c_g;

typedef struct UnkStruct_022dd12c_c {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022dd12c_c;

extern UnkStruct_022dd12c_w *data_ov000_02369b2c;
extern UnkStruct_022dd12c_p *data_ov000_0236ac60;
extern UnkStruct_022dd12c_g gDisplayMain;
extern UnkStruct_022dd12c_c data_022c4590;

void func_0204eb04(void);
void func_0204d5f8(void (*fn)(void));

void func_ov000_022dd12c(void) {
    UnkStruct_022dd12c_w *w = data_ov000_02369b2c;
    w->unk224++;
    data_ov000_0236ac60->unk3B2 = 0;
    data_ov000_0236ac60->unk5C |= 0x20;
    if (!(w->unk224 & 3)) {
        if (gDisplayMain.unk48 > 0)
            gDisplayMain.unk48--;
    }
    if (w->unk224 <= 0x12c)
        return;
    data_022c4590.unk10 = data_ov000_0236ac60->unk3AE * 1000;
    func_0204d5f8(func_0204eb04);
}
