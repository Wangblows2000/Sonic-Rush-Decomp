#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u16 unk2;
} UnkStruct_02309b60;

s32 MapObj8C_Create(UnkStruct_02309b60 *p, s32 a, s32 b, s32 c);
s32 MapObj8D_Create(UnkStruct_02309b60 *p, s32 a, s32 b, s32 c);
s32 GmkWall_Create(UnkStruct_02309b60 *p, s32 a, s32 b, s32 c);
s32 MapObj8F_Create(UnkStruct_02309b60 *p, s32 a, s32 b, s32 c);

s32 MapObj8C_CreateByType(UnkStruct_02309b60 *p, s32 a, s32 b, s32 c) {
    switch (p->unk2) {
    case 0x8c:
        return MapObj8C_Create(p, a, b, c);
    case 0x8d:
        return MapObj8D_Create(p, a, b, c);
    case 0x8e:
        return GmkWall_Create(p, a, b, c);
    case 0x8f:
        return MapObj8F_Create(p, a, b, c);
    }
    return 0;
}
