#include <nitro/types.h>

typedef struct UnkStruct_022d2920 {
    u8 pad[0x10];
    s32 unk10;
    u8 pad1[0x145c - 0x14];
    s32 unk145C;
    s32 unk1460;
    s32 unk1464;
    s32 unk1468;
} UnkStruct_022d2920;

void func_ov000_022d2920(UnkStruct_022d2920 *s, s32 mode) {
    if (mode == 0) {
        s->unk145C += s->unk1460;
        if (s->unk1464 < 0) {
            if (s->unk145C < s->unk1464)
                s->unk145C = s->unk1464;
        } else {
            if (s->unk145C > s->unk1464)
                s->unk145C = s->unk1464;
        }
    } else if (mode == 1) {
        s->unk145C -= s->unk1460;
        if (s->unk1464 < 0) {
            if (s->unk145C > s->unk1468)
                s->unk145C = s->unk1468;
        } else {
            if (s->unk145C < s->unk1468)
                s->unk145C = s->unk1468;
        }
    }
    s->unk10 += s->unk145C;
}
