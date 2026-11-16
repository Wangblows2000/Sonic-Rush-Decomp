#include <nitro/types.h>

typedef struct {
    u8 pad0[0x2E];
    s16 unk2E;
    u8 pad30[0x5C - 0x30];
    u16 unk5C;
    u8 pad5E[0x9C - 0x5E];
    u16 unk9C;
} UnkStruct_02321514;

extern u32 data_0208fe70;

s16 func_02068db4(s16 v, s32 a);
void func_02069818(void *w, u16 a);
void func_ov000_02321824(UnkStruct_02321514 *w);
void func_ov000_02321744(UnkStruct_02321514 *w, s16 x, s16 y, s32 a);

static inline u16 UnkRand_02321514(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov000_02321514(UnkStruct_02321514 *w) {
    w->unk2E = func_02068db4(w->unk2E, 8);
    if (!(w->unk5C & 8)) {
        return;
    }
    func_02069818(w, w->unk9C - 1);
    w->unk5C ^= 1;
    if (!(UnkRand_02321514() & 3)) {
        func_ov000_02321824(w);
        return;
    }
    if (w->unk5C & 1) {
        func_ov000_02321744(w, -0x81 - (UnkRand_02321514() & 0xfe), 0xff - (UnkRand_02321514() & 0x1fe), UnkRand_02321514() & 7);
    } else {
        func_ov000_02321744(w, 0x17f - (UnkRand_02321514() & 0xfe), 0xff - (UnkRand_02321514() & 0x1fe), UnkRand_02321514() & 7);
    }
}
