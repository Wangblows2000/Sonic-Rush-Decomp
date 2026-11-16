#include <nitro/types.h>

typedef struct {
    u8 pad0[0x20];
    u32 start;
    u32 end;
    u32 cur;
    u8 pad2c[0x18];
} UnkFile_02052af8;

typedef struct {
    u8 pad0[0x10];
    void *buf;
    u8 pad14[0x80];
    u8 bg94[0x40];
    u32 flags;
    u8 padd8[8];
    void (*fne0)(void);
    u8 pade4[0x1c];
    s32 fidx;
    s32 unk104;
} UnkWork_02052af8;

extern UnkFile_02052af8 *data_022c4664;

s32 FS_ReadFile(void *f, void *buf, u32 size);
void func_020526bc(void *bg, void *buf);
void func_02052d18(void);

#define REG_DB_DISPCNT (*(vu32 *)0x04001000)

void func_02052af8(UnkWork_02052af8 *w, s32 idx, s32 v) {
    if (w->fidx == idx) {
        w->flags |= 0x400000;
        w->fne0 = func_02052d18;
    } else {
        UnkFile_02052af8 *f;
        u32 plane;
        w->fidx = idx;
        f = &data_022c4664[idx];
        FS_ReadFile(f, w->buf, f->end - f->start);
        f->cur = f->start;
        func_020526bc(w->bg94, w->buf);
        plane = (REG_DB_DISPCNT & 0x1f00) >> 8;
        REG_DB_DISPCNT = (REG_DB_DISPCNT & ~0x1f00) | ((plane & ~1) << 8);
        w->flags |= 0x200000;
    }
    w->unk104 = v;
}
