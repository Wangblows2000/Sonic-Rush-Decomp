#include <nitro/types.h>

typedef struct UnkStruct_022dc6f4_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022dc6f4_v;

typedef struct UnkStruct_022dc6f4_a {
    u8 pad[0x1c];
    s32 unk1C;
    u8 pad1[0x48 - 0x20];
    UnkStruct_022dc6f4_v unk48;
    u8 pad2[0x138 - 0x54];
} UnkStruct_022dc6f4_a;

typedef struct UnkStruct_022dc6f4 {
    u8 pad[0x14];
    s32 unk14;
    u8 pad1[0x20c - 0x18];
    UnkStruct_022dc6f4_a unk20C;
    UnkStruct_022dc6f4_a unk344;
    u8 pad2[0x480 - 0x47c];
    s32 unk480;
    u8 pad3[0x488 - 0x484];
    void (*unk488)(void);
    s32 unk48C;
} UnkStruct_022dc6f4;

typedef struct UnkStruct_022dc6f4_w {
    u8 pad[0xac0];
    s32 unkAC0;
    u8 pad1[0xad0 - 0xac4];
    void *unkAD0;
} UnkStruct_022dc6f4_w;

extern UnkStruct_022dc6f4_w *data_ov000_02369b2c;

void func_ov000_022dc6cc(void);
void Model_SetResource(void *p, s32 a, s32 b);
void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

void func_ov000_022dc6f4(UnkStruct_022dc6f4 *s) {
    UnkStruct_022dc6f4_w *w = data_ov000_02369b2c;
    UnkStruct_022dc6f4_a *a;
    s->unk488 = func_ov000_022dc6cc;
    a = &s->unk20C;
    s->unk48C = 0;
    s->unk480 = 1;
    s->unk14 = 0x20000;
    a->unk48.y = -0x200000;
    a->unk1C = 0x34cc;
    Model_SetResource(a, w->unkAC0, 0);
    Model_SetAnimation(a, 1, w->unkAD0, 0, 0);
    s->unk344.unk48 = s->unk20C.unk48;
}
