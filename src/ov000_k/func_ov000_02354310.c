#include <nitro/types.h>

typedef struct {
    u8 pad0[0x14];
    s32 unk14;
    u8 pad18[0x58 - 0x18];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x68 - 0x5E];
    void (*unk68)(void);
    u8 pad6C[0x32A - 0x6C];
    u16 unk32A;
    u8 pad32C[0x384 - 0x32C];
    u8 unk384[0x54];
} UnkStruct_02354310;

typedef struct {
    u8 pad0[0x62];
    u16 unk62;
} UnkStruct_0236aad8;

extern UnkStruct_0236aad8 data_ov000_0236aad8;

void func_ov000_02354174(void);
void func_ov000_0234c9e8(UnkStruct_02354310 *w);
void func_ov000_0234b63c(UnkStruct_02354310 *w);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);

void func_ov000_02354310(UnkStruct_02354310 *w) {
    if (w->unk68 == func_ov000_02354174) {
        return;
    }
    func_ov000_0234c9e8(w);
    func_ov000_0234b63c(w);
    w->unk5C |= 4;
    w->unk58 |= 0x10;
    w->unk58 &= ~0x80;
    w->unk14 = (data_ov000_0236aad8.unk62 - 0xF) << 8;
    w->unk68 = func_ov000_02354174;
    w->unk32A = 8;
    Sound_PlaySeqArcEx(w->unk384, 0, 0x54);
}
