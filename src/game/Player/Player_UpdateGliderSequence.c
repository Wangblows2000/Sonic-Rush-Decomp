#include <nitro/types.h>

typedef struct {
    u32 unk0;
    u8 pad4[0x60];
} UnkStruct_0234eaec_Cam;

typedef struct {
    u8 pad0[0x2E];
    s16 unk2E;
    s16 unk30;
    u8 pad32[0x58 - 0x32];
    u32 unk58;
    u8 pad5C[0x327 - 0x5C];
    u8 unk327;
    u8 pad328[0x420 - 0x328];
    u16 unk420;
} UnkStruct_0234eaec;

extern UnkStruct_0234eaec_Cam gCamera[];

s16 ObjSpd_DownSet(s16 v, s32 a);
s16 ObjSpd_UpSet(s16 v, s32 a, s32 b);
void Player_ClampToGliderArea(UnkStruct_0234eaec *w);

void Player_UpdateGliderSequence(UnkStruct_0234eaec *w) {
    gCamera[w->unk327].unk0 &= ~1;
    gCamera[w->unk327].unk0 &= ~2;
    if (w->unk420 & 0x30) {
        if (w->unk420 & 0x10) {
            if (w->unk2E < 0) {
                w->unk2E = ObjSpd_DownSet(w->unk2E, 0x48);
            }
            if (!(w->unk58 & 4)) {
                w->unk2E = ObjSpd_UpSet(w->unk2E, 0x28, 0x300);
            }
        } else {
            if (w->unk2E > 0) {
                w->unk2E = ObjSpd_DownSet(w->unk2E, 0x48);
            }
            if (!(w->unk58 & 8)) {
                w->unk2E = ObjSpd_UpSet(w->unk2E, -0x28, 0x300);
            }
        }
    } else {
        w->unk2E = ObjSpd_DownSet(w->unk2E, 0x48);
    }
    if (w->unk420 & 0xc0) {
        if (w->unk420 & 0x80) {
            if (w->unk30 < 0) {
                w->unk30 = ObjSpd_DownSet(w->unk30, 0x48);
            }
            if (!(w->unk58 & 1)) {
                w->unk30 = ObjSpd_UpSet(w->unk30, 0x28, 0x300);
            }
        } else {
            if (w->unk30 > 0) {
                w->unk30 = ObjSpd_DownSet(w->unk30, 0x48);
            }
            if (!(w->unk58 & 2)) {
                w->unk30 = ObjSpd_UpSet(w->unk30, -0x28, 0x300);
            }
        }
    } else {
        w->unk30 = ObjSpd_DownSet(w->unk30, 0x48);
    }
    Player_ClampToGliderArea(w);
}
