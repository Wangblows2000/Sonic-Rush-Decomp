#include <nitro/types.h>

typedef struct {
    u8 x;
    u8 y;
    u16 type;
} UnkEnt_023263c0;

typedef s32 (*UnkFunc_023263c0)(UnkEnt_023263c0 *e, s32 x, s32 y, u32 a);

extern UnkEnt_023263c0 gDecorationSlots[];
extern u8 data_ov000_0236a671[];
extern u16 data_ov000_0236a672[];
extern UnkFunc_023263c0 gDecorationCreateTable[];

s32 Decoration_AllocSlot(void);
void Decoration_FreeSlot(UnkEnt_023263c0 *e);

s32 Decoration_Spawn(s32 type, s32 x, s32 y) {
    s32 r = 0;
    s32 i = Decoration_AllocSlot();
    UnkFunc_023263c0 fn;
    UnkEnt_023263c0 *e;

    if (i < 32) {
        fn = gDecorationCreateTable[type];
        e = &gDecorationSlots[i];
        gDecorationSlots[i].x = 0xff;
        gDecorationSlots[i].y = 0xff;
        gDecorationSlots[i].type = type;
        r = fn(e, x, y, 0);
        if (r == 0)
            Decoration_FreeSlot(e);
    }
    return r;
}
