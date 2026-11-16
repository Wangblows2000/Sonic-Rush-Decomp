#include <nitro/types.h>

typedef struct {
    u8 pad0[0x29];
    u8 unk29;
    u8 type;
    u8 unk2b;
    u8 unk2c;
    u8 pad2d;
    u16 id;
} UnkEntry_022c93e0;

typedef struct {
    s32 active;
    s32 unk4;
    s32 unk8;
    s32 unkc;
    s32 unk10;
    s32 cur;
    s32 idx;
    u8 pad1c[0x24 - 0x1c];
    s32 unk24;
    u8 pad28[0x180 - 0x28];
    s32 unk180;
    u8 pad184[0x1a4 - 0x184];
    u8 unk1a4[4];
    s32 unk1a8;
    u16 unk1ac;
    u16 unk1ae;
    s32 unk1b0;
} UnkStruct_022c93e0;

extern UnkEntry_022c93e0 *data_ov001_0231f048;
extern u8 data_ov001_0231abc8[];
extern const char *data_ov001_0231ab44[];
extern void *gSoundHeap;

void Sound_ResetHeap(void);
void func_0202a2f0(char *buf, const void *fmt, const char *s);
void func_02036854(char *buf);
void func_0201e7cc(u8 a, void *b);
s32 File_Load(char *buf, s32 a);
void func_02036820(s32 a);
void func_020203bc(s32 a, void *b);
void func_020201b0(void *a);
void func_02020230(void *a, u16 id, s32 b);
void Sound_PlaySeq(s32 a, u16 id);
u8 *func_0201d534(u16 id);
void Sound_PlaySeqArc(s32 a, u8 b, u16 id);
void func_020366f4(s32 a, u8 b, u16 id);
void func_0201cf4c(s32 a, s32 b, s32 c, s32 d, s32 e, void (*fn)(void *), void *arg);
void func_ov001_022c8f30(void *p);

#pragma thumb on
void func_ov001_022c93e0(UnkStruct_022c93e0 *p, s32 idx) {
    UnkEntry_022c93e0 *e;
    u8 *d;
    char buf[0x24];

    e = &data_ov001_0231f048[idx];
    Sound_ResetHeap();
    if (p->active) {
        if (p->cur != p->idx) p->unk24 = 0;
    } else {
        if (p->cur != p->idx) p->unk24 = 0;
    }
    p->active = 1;
    p->idx = idx;
    p->unk1a8 = 0;
    p->unk1ac = 0;
    p->unk1ae = 0;
    p->unk8 = 0;
    p->unkc = 0;
    func_0202a2f0(buf, data_ov001_0231abc8, data_ov001_0231ab44[e->unk29]);
    if (e->unk29 == 0) {
        func_02036854(buf);
        func_0201e7cc(e->unk2b, gSoundHeap);
    } else {
        p->unk1b0 = File_Load(buf, -1);
        func_02036820(p->unk1b0);
    }
    switch (e->type) {
    case 0:
        if (e->unk29 == 0 && e->unk2b == 0xe) {
            func_020203bc(10, gSoundHeap);
            func_020201b0(p->unk1a4);
            func_02020230(p->unk1a4, e->id, 0);
            p->unk4 = 1;
            p->unk8 = 1;
        } else {
            Sound_PlaySeq(0, e->id);
            p->unk4 = 0;
            p->unkc = 1;
            d = func_0201d534(e->id);
            if (d == NULL) {
                p->unk10 = 0;
            } else {
                p->unk10 = d[2];
            }
        }
        break;
    case 1:
        Sound_PlaySeqArc(0, e->unk2c, e->id);
        p->unk4 = 0;
        p->unk8 = 1;
        break;
    case 2:
        func_020366f4(0, e->unk2c, e->id);
        p->unk4 = 0;
        p->unk8 = 1;
        break;
    }
    if (p->unk8 != 0) {
        func_0201cf4c(p->unk180, 0x80, 1, 0x1f40, 2, func_ov001_022c8f30, p);
    }
}
#pragma thumb reset
