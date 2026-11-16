#include <nitro/types.h>

typedef struct {
    u8 pad0[0xc];
    void *unkc;
    u8 pad10[0xc4];
    u32 flags;
} UnkWork_020529d8;

typedef struct {
    u8 pad0[0x10];
    UnkWork_020529d8 *work;
} UnkTask_020529d8;

extern UnkTask_020529d8 *data_022c4660;
extern UnkTask_020529d8 *data_022b4574;

void func_020371c8(UnkTask_020529d8 *t);
void func_020524d4(void *p);

#define REG_DB_DISPCNT (*(vu32 *)0x04001000)
#define REG_DB_BG1CNT (*(vu16 *)0x0400100a)

void func_020529d8(void) {
    UnkWork_020529d8 *w;
    u32 sb;
    u32 cb;
    u32 plane;

    if (data_022c4660 == NULL)
        goto kill;
    w = data_022c4660->work;
    if (w->flags & 0x20000) {
        if (w->flags & 0x10000) {
            sb = 7;
            cb = 2;
        } else {
            sb = 0xa;
            cb = 2;
        }
        REG_DB_BG1CNT = (REG_DB_BG1CNT & 0x43) | (sb << 8) | (cb << 2) | 0x2000;
        w->flags &= ~0x20000;
        w->flags ^= 0x10000;
        func_020524d4(w->unkc);
    }
    if (w->flags & 0x400000) {
        plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
        REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | ((plane | 1) << 8);
        w->flags &= ~0x400000;
    } else if (w->flags & 0x800000) {
        plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
        REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | ((plane & ~1) << 8);
        w->flags &= ~0x800000;
    }
    return;
kill:
    func_020371c8(data_022b4574);
}
