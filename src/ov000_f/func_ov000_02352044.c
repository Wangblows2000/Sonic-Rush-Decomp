#include <nitro/types.h>

typedef struct UnkStruct_02352044 UnkStruct_02352044;

struct UnkStruct_02352044 {
    u8 pad0;
    u8 unk1;
    u8 pad2[4 - 2];
    u8 unk4;
    u8 pad5[0x28 - 5];
    s16 unk28;
    u8 pad2A[0x2E - 0x2A];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x34 - 0x32];
    s16 unk34;
    u8 pad36[0x3C - 0x36];
    s16 unk3C;
    u8 pad3E[0x58 - 0x3E];
    u32 unk58;
    u16 unk5C;
    u8 pad5E[0x324 - 0x5E];
    u8 unk324;
    u8 pad325[0x384 - 0x325];
    u8 unk384[4];
};

typedef struct {
    s16 v;
    u8 pad2[0x30 - 2];
} UnkPhys_02352044;

typedef struct {
    u8 pad0[0x20];
    u32 unk20;
} UnkStruct_02352044_4560;

extern s16 data_0206e470[];
extern UnkPhys_02352044 data_ov000_02368ba0[];
extern UnkStruct_02352044_4560 data_022c4560;

#define IDX_02352044(a) ((s32)((a) & 0xffff) >> 4)
#define SIN_02352044(a) data_0206e470[(u32)(IDX_02352044(a) << 1)]
#define COS_02352044(a) data_0206e470[(u32)((IDX_02352044(a) << 1) + 1)]

void func_ov000_02328bf0(UnkStruct_02352044 *w, s32 a, s32 b);
void Sound_PlaySeqArcEx(void *p, s32 a, s32 b);
void func_ov000_0234a064(UnkStruct_02352044 *w, u8 ang);

void func_ov000_02352044(UnkStruct_02352044 *w) {
    if (w->unk1 > 0x80) {
        w->unk5C |= 1;
    } else {
        w->unk5C &= ~1;
    }
    if (w->unk58 & 1) {
        if ((w->unk28 < 0 && (w->unk5C & 1)) || (w->unk28 > 0 && !(w->unk5C & 1))) {
            if (!(data_022c4560.unk20 & 7)) {
                func_ov000_02328bf0(w, 8, 0x10);
            }
        }
    }
    if (!(data_022c4560.unk20 & 0x3f)) {
        Sound_PlaySeqArcEx(w->unk384, 0, 0x5c);
    }
    if (w->unk58 & 0x10) {
        if (!(w->unk58 & 1)) {
            return;
        }
        w->unk58 &= ~0x10;
        w->unk58 &= ~0x8000;
        w->unk2E = w->unk34;
        func_ov000_0234a064(w, 0);
        w->unk3C = data_ov000_02368ba0[w->unk324].v;
        w->unk4 = 0;
        w->unk34 >>= 1;
    } else {
        if (w->unk58 & 1) {
            return;
        }
        w->unk58 |= 0x10;
        w->unk58 |= 0x8000;
        w->unk2E = (w->unk34 * COS_02352044((u16)(w->unk1 << 8))) >> 12;
        w->unk30 = (w->unk34 * SIN_02352044((u16)(w->unk1 << 8))) >> 12;
    }
}
