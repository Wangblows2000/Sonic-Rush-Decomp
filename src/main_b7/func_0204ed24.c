#include <nitro/types.h>

typedef struct {
    s32 unk0;
    u8 pad4[0x24];
    u16 unk28;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[2];
    u8 b2;
    u8 b3;
    u8 pad4[0x56];
    u8 b5a;
} UnkSave_022c468c;

typedef struct {
    u8 pad0[0x14];
    u8 b14;
} UnkStruct_022c4590;

extern UnkStruct_022c4560 gGameState;
extern UnkSave_022c468c data_022c468c;
extern UnkStruct_022c4590 data_022c4590;

void func_0204ed24(void) {
    UnkSave_022c468c *s = &data_022c468c;
    if (gGameState.unk0 != 1) {
        s->b2 = data_022c4590.b14;
        s->b3 = gGameState.unk28;
    } else {
        s->b5a = data_022c4590.b14;
    }
}
