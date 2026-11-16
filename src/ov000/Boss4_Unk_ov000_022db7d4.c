#include <nitro/types.h>

typedef struct UnkStruct_022db7d4 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
} UnkStruct_022db7d4;

void Boss4_Unk_ov000_022db758(s32 a, s32 b, s32 c, s32 d);

void Boss4_Unk_ov000_022db7d4(s32 a, s32 b, UnkStruct_022db7d4 *s) {
    Boss4_Unk_ov000_022db758(a, b, s->unk10, s->unk14);
}
