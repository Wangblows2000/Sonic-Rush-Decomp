#include <nitro/types.h>

typedef struct UnkStruct_022e023c {
    u8 pad0[4];
    u32 unk4;
    u8 pad8[0x10 - 8];
    s32 unk10;
    u8 cam[0x454 - 0x14];
    u8 unk454[0xbd4 - 0x454];
    struct {
        u8 unk0[0x10];
    } unkBD4;
} UnkStruct_022e023c;

extern u32 gRandSeed;

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov001_022dfeec(void);
void func_ov001_022dfe20(void);
void func_ov001_022dfb48(UnkStruct_022e023c *w, void *cam);
void func_ov001_022dfa3c(UnkStruct_022e023c *w, void *cam);
void func_ov001_022df5f0(UnkStruct_022e023c *w);
void func_ov001_022de1e0(UnkStruct_022e023c *w, void *a);
void func_ov001_022df3d8(UnkStruct_022e023c *w, void *a);
void func_ov001_022df3a4(UnkStruct_022e023c *w, void *a);
void func_ov001_022df474(UnkStruct_022e023c *w);

void func_ov001_022e023c(UnkStruct_022e023c *w) {
    if (w->unk4 & 1) {
        func_ov001_022dfeec();
        func_ov001_022dfb48(w, w->cam);
    } else {
        func_ov001_022dfe20();
        func_ov001_022dfa3c(w, w->cam);
    }
    w->unk4 ^= 1;
    if (w->unk10 == 2) {
        if ((Rand() & 7) == 0)
            func_ov001_022df5f0(w);
    } else {
        if ((Rand() & 3) == 0)
            func_ov001_022df5f0(w);
    }
    func_ov001_022de1e0(w, w->unk454);
    func_ov001_022df3d8(w, &w->unkBD4);
    func_ov001_022df3a4(w, w->unkBD4.unk0);
    func_ov001_022df474(w);
}
