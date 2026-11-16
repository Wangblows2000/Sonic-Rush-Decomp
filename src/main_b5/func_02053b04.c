#include <nitro/types.h>

typedef struct {
    s32 mode;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0xc];
    u16 unkc;
    u8 pade[0x56];
    u16 unk64;
} UnkStruct_022c4680;

typedef struct {
    s32 mode;
    u8 pad4[0xd0];
    u32 flags;
} UnkWork_02053b04;

typedef struct {
    u8 pad0[0x10];
    UnkWork_02053b04 *work;
} UnkTask_02053b04;

extern UnkTask_02053b04 *data_022c4660;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_022c4680 data_022c4680;

#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

void func_02053b04(BOOL on) {
    UnkWork_02053b04 *w;
    u32 plane;

    if (data_022c4660 == NULL)
        return;
    w = data_022c4660->work;
    if (w->mode == 0) {
        if (data_022c4560.mode == 0 && data_022c4680.unkc < 3)
            return;
        if (data_022c4560.mode == 1) {
            if (data_022c4680.unk64 < 4)
                return;
            if (data_022c4680.unk64 >= 0x1b && data_022c4680.unk64 < 0x1c)
                return;
        }
    }
    if (on) {
        w->flags |= 1;
        plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
        REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | ((plane | 2) << 8);
    } else {
        w->flags &= ~1;
        plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
        REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | ((plane & ~2) << 8);
    }
}
