#include <nitro/types.h>

typedef struct {
    u8 x;
    u8 y;
    u16 type;
} UnkEnt_023263c0;

typedef s32 (*UnkFunc_023263c0)(UnkEnt_023263c0 *e, s32 x, s32 y, u32 a);

extern UnkEnt_023263c0 data_ov000_0236a670[];
extern u8 data_ov000_0236a671[];
extern u16 data_ov000_0236a672[];
extern UnkFunc_023263c0 data_ov000_023645c4[];

s32 func_ov000_02326498(void);
void func_ov000_0232644c(UnkEnt_023263c0 *e);

s32 func_ov000_023263c0(s32 type, s32 x, s32 y) {
    s32 r = 0;
    s32 i = func_ov000_02326498();
    UnkFunc_023263c0 fn;
    UnkEnt_023263c0 *e;

    if (i < 32) {
        fn = data_ov000_023645c4[type];
        e = &data_ov000_0236a670[i];
        data_ov000_0236a670[i].x = 0xff;
        data_ov000_0236a670[i].y = 0xff;
        data_ov000_0236a670[i].type = type;
        r = fn(e, x, y, 0);
        if (r == 0)
            func_ov000_0232644c(e);
    }
    return r;
}
