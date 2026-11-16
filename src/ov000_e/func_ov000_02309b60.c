#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u16 unk2;
} UnkStruct_02309b60;

s32 func_ov000_023099e8(UnkStruct_02309b60 *p, s32 a, s32 b, s32 c);
s32 func_ov000_023092e4(UnkStruct_02309b60 *p, s32 a, s32 b, s32 c);
s32 func_ov000_02308eb8(UnkStruct_02309b60 *p, s32 a, s32 b, s32 c);
s32 func_ov000_02308904(UnkStruct_02309b60 *p, s32 a, s32 b, s32 c);

s32 func_ov000_02309b60(UnkStruct_02309b60 *p, s32 a, s32 b, s32 c) {
    switch (p->unk2) {
    case 0x8c:
        return func_ov000_023099e8(p, a, b, c);
    case 0x8d:
        return func_ov000_023092e4(p, a, b, c);
    case 0x8e:
        return func_ov000_02308eb8(p, a, b, c);
    case 0x8f:
        return func_ov000_02308904(p, a, b, c);
    }
    return 0;
}
