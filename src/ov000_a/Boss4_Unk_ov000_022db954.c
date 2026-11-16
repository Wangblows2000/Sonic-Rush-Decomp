#include <nitro/types.h>

typedef struct UnkStruct_022db954 {
    u8 pad[0xbbc];
    s32 unkBBC;
    u8 pad1[0xbc4 - 0xbc0];
    s32 unkBC4;
    s32 unkBC8;
    s32 unkBCC;
} UnkStruct_022db954;

void Boss4_Unk_ov000_022db954(UnkStruct_022db954 *s, s32 t) {
    if (s->unkBBC > t) {
        s->unkBBC -= s->unkBC4;
        if (s->unkBBC <= t) {
            s->unkBBC = t;
            s->unkBC4 = 0;
        }
    } else {
        s->unkBBC += s->unkBC4;
        if (s->unkBBC >= t) {
            s->unkBBC = t;
            s->unkBC4 = 0;
        }
    }
}
