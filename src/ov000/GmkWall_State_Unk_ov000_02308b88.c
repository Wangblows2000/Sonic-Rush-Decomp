#include <nitro/types.h>

typedef struct UnkStruct_02308b88_2 {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_02308b88_2;

typedef struct UnkStruct_02308b88 {
    u8 pad[0x54];
    u32 unk54;
    u8 pad1[0x1f4 - 0x58];
    u32 unk1F4;
} UnkStruct_02308b88;

u8 func_ov000_02307e84(UnkStruct_02308b88 *s);
UnkStruct_02308b88_2 *func_ov000_0234237c(s32 a, s32 i);
BOOL func_ov000_02307dfc(UnkStruct_02308b88_2 *p, s32 a);
void GmkWall_Action_Unk_ov000_02308b64(UnkStruct_02308b88 *s);

void GmkWall_State_Unk_ov000_02308b88(UnkStruct_02308b88 *s) {
    UnkStruct_02308b88_2 *p = func_ov000_0234237c(1, func_ov000_02307e84(s));
    if (func_ov000_02307dfc(p, 0)) {
        s->unk1F4 &= ~0x10;
        s->unk54 |= 4;
        return;
    }
    if (p == NULL) {
        s->unk54 |= 4;
        return;
    }
    if (!(p->unk20C & 0x40))
        return;
    GmkWall_Action_Unk_ov000_02308b64(s);
}
