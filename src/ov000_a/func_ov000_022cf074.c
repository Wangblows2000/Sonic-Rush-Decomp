#include <nitro/types.h>

typedef struct UnkStruct_022cf074_a {
    u8 pad[0x1c];
    s32 unk1C;
    u8 pad1[0x4c - 0x20];
    s32 unk4C;
    u8 pad2[0x10c - 0x50];
    u8 unk10C[4];
    u8 pad3[0x114 - 0x110];
    s32 unk114;
    u8 pad4[0x138 - 0x118];
} UnkStruct_022cf074_a;

typedef struct UnkStruct_022cf074_s {
    u8 pad[0x5bc];
    UnkStruct_022cf074_a unk5BC;
    UnkStruct_022cf074_a unk6F4;
    UnkStruct_022cf074_a unk82C;
    u8 pad2[0xf7c - 0x82c - 0x138];
    s32 unkF7C;
    s32 unkF80;
    u8 pad3[0x1420 - 0xf84];
    UnkStruct_022cf074_a *unk1420;
    u8 pad4[0x1454 - 0x1424];
    s32 unk1454;
} UnkStruct_022cf074_s;

typedef struct UnkStruct_022cf074 UnkStruct_022cf074;
struct UnkStruct_022cf074 {
    UnkStruct_022cf074_s *unk0;
    void (*func)(UnkStruct_022cf074 *t);
    s32 unk8;
    s32 unkC;
};

typedef struct UnkStruct_022cf074_cam {
    u8 pad[0x3c];
    s32 unk3C;
    s32 unk40;
} UnkStruct_022cf074_cam;

extern UnkStruct_022cf074_cam data_ov000_0236a444;

s32 func_02002094(s32 a, s32 b);
void func_02031a68(void *p, s32 a, s32 b, s32 c, s32 d);

typedef struct UnkStruct_022cf074_task {
    u8 pad[0x10];
    UnkStruct_022cf074 *unk10;
} UnkStruct_022cf074_task;

typedef struct UnkStruct_022cf074_b {
    u8 pad[0x210];
    UnkStruct_022cf074_task *unk210;
} UnkStruct_022cf074_b;

void func_ov000_022cefa0(UnkStruct_022cf074 *t);
void func_02036754(s32 a, s32 b, s32 c);

void func_ov000_022cf074(UnkStruct_022cf074_s *s) {
    UnkStruct_022cf074_a *b;
    ((UnkStruct_022cf074_b *)s)->unk210->unk10->func = func_ov000_022cefa0;
    b = &s->unk6F4;
    func_02031a68(b, 0, s->unkF7C, 1, 0);
    b->unk10C[0] &= ~4;
    func_02031a68(b, 3, s->unkF80, 1, 0);
    b->unk10C[3] &= ~4;
    b->unk114 = 0x1000;
    s->unk1420 = b;
    func_02036754(s->unk1454, 0, 0x9f);
}
