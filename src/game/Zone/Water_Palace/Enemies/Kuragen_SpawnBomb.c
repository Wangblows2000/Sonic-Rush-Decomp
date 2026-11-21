#include <nitro/types.h>

typedef struct {
    u8 pad0[4];
    u16 unk4;
} UnkStruct_02301390_1e0;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad18[0x1e0 - 0x18];
    UnkStruct_02301390_1e0 *unk1E0;
} UnkStruct_02301390;

typedef struct {
    u8 pad0[0x88];
    UnkStruct_02301390 *unk88;
} UnkStruct_02301390_e;

UnkStruct_02301390_e *MapObject_Spawn(s32 a, s32 b, s32 c, u16 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void KuragenBomb_Attach(UnkStruct_02301390_e *e);


void Kuragen_SpawnBomb(UnkStruct_02301390 *p) {
    UnkStruct_02301390_e *e;
    u16 f = 0;

    f |= p->unk1E0->unk4 & 0xc;
    e = MapObject_Spawn(0xe3, p->unk10, p->unk14, f, 0, 0, 0, 0, 0);
    e->unk88 = p;
    KuragenBomb_Attach(e);
}
