#include <nitro/types.h>

typedef struct {
    u8 pad0[0x10];
    s32 x;
    u8 pad14[0x1C - 0x14];
    s32 unk1C;
    u8 pad20[0x28 - 0x20];
    s16 unk28;
    u8 pad2A[0x2E - 0x2A];
    s16 unk2E;
    u8 pad30[0x34 - 0x30];
    s16 unk34;
    s16 unk36;
    u8 pad38[0x58 - 0x38];
    u32 unk58;
    u8 pad5C[0x32C - 0x5C];
    u32 unk32C;
    u8 pad330[0x3BE - 0x330];
    s16 unk3BE;
    u8 pad3C0[0x3E8 - 0x3C0];
    void *unk3E8;
} UnkStruct_02342b78_P;

typedef struct {
    u8 pad0[0x18];
    UnkStruct_02342b78_P *ply;
} UnkStruct_02342b78;

typedef struct {
    u8 pad0[0x18];
    u32 unk18;
} UnkStruct_02342b78_4560;

extern UnkStruct_02342b78_4560 gGameState;

void func_ov000_0232fc10(UnkStruct_02342b78_P *p);

#define ABS_02342b78(x) ((x) < 0 ? -(x) : (x))

void Player_OnPushRival(UnkStruct_02342b78 *a, UnkStruct_02342b78 *b) {
    UnkStruct_02342b78_P *me = b->ply;
    s32 v;
    BOOL ok = TRUE;
    UnkStruct_02342b78_P *o = a->ply;
    s32 d;
    s32 ad;
    s32 a2;
    s32 ox;
    s32 mx;

    if (me->unk32C & 0x80000) {
        return;
    }
    if (o->unk32C & 0x80000) {
        return;
    }
    if (!(gGameState.unk18 & 1)) {
        return;
    }
    if (me->unk32C & 0x100000) {
        return;
    }
    if (me->unk58 & 0x2100) {
        return;
    }
    if (o->unk3E8 != NULL) {
        return;
    }
    if (me->unk3E8 != NULL) {
        return;
    }
    if (me->unk3BE != 0) {
        return;
    }
    v = (s16)(me->unk2E + me->unk36);
    if (!(me->unk58 & 0x8000)) {
        v = (s16)(v + me->unk34);
    }
    if ((v > 0 && me->x > o->x) || (v < 0 && me->x < o->x)) {
        v = 0;
    }
    if ((me->unk1C > o->unk1C && me->x < o->x) || (me->unk1C < o->unk1C && me->x > o->x)) {
        ok = FALSE;
        me->unk2E = 0;
        me->unk34 = 0;
        v = (s16)(v << 1);
    }
    if (ABS_02342b78(me->x - o->x) < 0x2800 && ((me->unk28 < -0x800 && o->unk28 > 0x800) || (me->unk28 > 0x800 && o->unk28 < -0x800))) {
        ok = FALSE;
        me->unk2E = 0;
        me->unk34 = 0;
        v = (s16)(v << 1);
    }
    d = me->x - o->x;
    if (ABS_02342b78(d) < 0x1800) {
        if (ABS_02342b78(d) > 0x1000) {
            v = (v * (0x800 - (ABS_02342b78(d) - 0x1000)) << 5) >> 16;
        }
        if (o->unk28 == 0) {
            if (me->x > o->x && v > -0x100 && v < 0x80) {
                v = -0x100;
            }
            if (me->x < o->x && v < 0x100 && v > -0x80) {
                v = 0x100;
            }
        }
    } else if (ok) {
        v = 0;
    }
    me->unk36 -= v;
    if (v == 0) {
        return;
    }
    if (!(me->unk32C & 0x10000)) {
        func_ov000_0232fc10(me);
    }
    me->unk32C |= 0x10000;
}
