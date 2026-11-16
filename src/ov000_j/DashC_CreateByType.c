#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u16 unk2;
} UnkStruct_0230787c;

s32 DashC_Create(UnkStruct_0230787c *p, s32 a, s32 b, s32 c);
s32 DashCT_Create(UnkStruct_0230787c *p, s32 a, s32 b, s32 c);

s32 DashC_CreateByType(UnkStruct_0230787c *p, s32 a, s32 b, s32 c) {
    switch (p->unk2) {
    case 0x6a:
    case 0x6b:
    case 0x6c:
    case 0x6d:
        return DashC_Create(p, a, b, c);
    case 0x63:
    case 0x64:
    case 0x65:
    case 0x66:
        return DashCT_Create(p, a, b, c);
    }
    return 0;
}
