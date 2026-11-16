#include <nitro/types.h>

typedef struct UnkStruct_022d6688_a {
    u8 pad[0x10c];
    u8 unk10C;
    u8 pad1[0x114 - 0x10d];
    s32 unk114;
    u8 pad2[0x170 - 0x118];
} UnkStruct_022d6688_a;

typedef struct UnkStruct_022d6688 UnkStruct_022d6688;
struct UnkStruct_022d6688 {
    u8 pad[0x14];
    s32 unk14;
    u8 padA[0x2e - 0x18];
    s16 unk2E;
    u16 unk30;
    u8 pad0[0x5c - 0x32];
    u16 unk5C;
    u8 pad1[0x68 - 0x5e];
    void (*func)(UnkStruct_022d6688 *s);
    u8 pad2[0x258 - 0x6c];
    u16 unk258;
    u16 unk25A;
    u8 pad3[0x264 - 0x25c];
    u16 unk264;
    u8 pad4[0x2c0 - 0x266];
    s32 unk2C0;
    UnkStruct_022d6688_a unk2C4;
    void *unk434;
};

void Model_SetAnimation(void *p, s32 a, void *b, s32 c, s32 d);

typedef struct UnkStruct_022d6688_c {
    u8 pad[0xdc];
    s32 unkDC;
} UnkStruct_022d6688_c;

extern UnkStruct_022d6688_c data_ov000_0236a444;

void Boss3_ResetCamera(void);
void Boss3PartCC_State_CatchBallDone(UnkStruct_022d6688 *s);

void Boss3PartCC_State_CatchBallWait(UnkStruct_022d6688 *s) {
    if (!(s->unk2C4.unk10C & 2))
        return;
    if (data_ov000_0236a444.unkDC != 2)
        Boss3_ResetCamera();
    s->func = Boss3PartCC_State_CatchBallDone;
}
