#include <nitro/types.h>

typedef struct UnkStruct_022dcf38_m43 {
    s32 m[12];
} UnkStruct_022dcf38_m43;

typedef struct UnkStruct_022dcf38_a {
    u8 pad[0x24];
    UnkStruct_022dcf38_m43 unk24;
    u8 pad1[0x138 - 0x54];
} UnkStruct_022dcf38_a;

typedef struct UnkStruct_022dcf38 {
    u8 pad[0x5d0];
    UnkStruct_022dcf38_a unk5D0;
    u8 pad1[0xb28 - 0x708];
    UnkStruct_022dcf38_m43 unkB28;
    u8 pad2[0xbbc - 0xb58];
    s32 unkBBC;
} UnkStruct_022dcf38;

typedef struct UnkStruct_022dcf38_s {
    u8 pad[0x20c];
    UnkStruct_022dcf38_a *unk20C;
} UnkStruct_022dcf38_s;

typedef struct UnkTask_022dcf38 {
    u8 pad[0x10];
    UnkStruct_022dcf38_s *unk10;
} UnkTask_022dcf38;

extern UnkTask_022dcf38 *gTaskCurrent;
extern UnkStruct_022dcf38 *data_ov000_02369b2c;
extern s16 data_0206e470[];

void func_02000e98(void *m, s32 s, s32 c);
void func_02000f34(void *a, void *b, void *c);
void Model_Animate(void *a);
void Model_Draw(void *a);

void Boss4PartCF_Draw(void) {
    u8 mtx[0x24];
    UnkStruct_022dcf38 *w = data_ov000_02369b2c;
    UnkStruct_022dcf38_a *a = gTaskCurrent->unk10->unk20C;
    u16 ang;
    a->unk24 = w->unkB28;
    ang = -w->unkBBC;
    func_02000e98(mtx, data_0206e470[((ang & 0xffff) >> 4) * 2], data_0206e470[((ang & 0xffff) >> 4) * 2 + 1]);
    func_02000f34(mtx, &a->unk24, &a->unk24);
    Model_Animate(a);
    Model_Draw(a);
}
