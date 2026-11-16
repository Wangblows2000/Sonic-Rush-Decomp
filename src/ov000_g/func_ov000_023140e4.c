#include <nitro/types.h>

typedef struct {
    u8 pad0;
    u8 unk1;
    u8 pad2[0x2E - 2];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x4A - 0x32];
    s16 unk4A;
    u8 pad4C[0x58 - 0x4C];
    u32 unk58;
    u8 pad5C[0x62 - 0x5C];
    u16 unk62;
    u8 pad64[0x1DC - 0x64];
    void *unk1DC;
} UnkStruct_023140e4;

extern void *data_ov000_0236ac60;
extern s16 data_0206e470[];

#define SIN(a) data_0206e470[(((a) & 0xffff) >> 4) * 2]

void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void Sound_SetPositional(void *a, void *b, void *c);

void func_ov000_023140e4(UnkStruct_023140e4 *w) {
    if (w->unk58 & 0x100)
        return;
    if (!(w->unk58 & 1))
        return;
    Sound_PlaySeqArcEx(w->unk1DC, 0, 0x5f);
    Sound_SetPositional(w->unk1DC, w, data_ov000_0236ac60);
    w->unk62 |= 0x44;
    w->unk58 |= 0x100;
    w->unk58 &= ~1;
    w->unk2E = (SIN((u16)(w->unk1 << 8)) * 0x280) >> 12;
    w->unk30 = -0x480;
    w->unk4A = 0x20;
}
