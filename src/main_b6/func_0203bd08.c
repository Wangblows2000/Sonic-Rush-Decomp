#include <nitro/types.h>

typedef struct {
    u32 *ptr;
    u32 size;
} UnkSeg_0203bd08;

extern char data_02078064[];

void *func_0200b828(void *name, u32 len);
BOOL func_0200b034(void *p);
void func_0200aee8(void *arc, void *proc, u32 flags);
BOOL func_0200b0fc(void *arc, u32 base, void *fat, u32 fatSize, void *fnt, u32 fntSize, void *read, void *write);
void func_0203c190(void);
void func_0203c220(void);
void func_0203c218(void);

u32 func_0203bd08(u32 *p) {
    u32 r;
    void *arc;
    s32 i;
    UnkSeg_0203bd08 seg[4];

    arc = func_0200b828(data_02078064, 3);
    func_0200b034(arc);
    func_0200aee8(arc, func_0203c190, -1);
    r = *p++;
    for (i = 0; i < 4; i++) {
        seg[i].ptr = p + 1;
        seg[i].size = *p;
        p = (u32 *)((u8 *)seg[i].ptr + ((seg[i].size + 0x1f) & ~0x1f));
    }
    func_0200b0fc(arc, 0, seg[0].ptr, seg[0].size, seg[1].ptr, seg[1].size, func_0203c220, func_0203c218);
    return r;
}
