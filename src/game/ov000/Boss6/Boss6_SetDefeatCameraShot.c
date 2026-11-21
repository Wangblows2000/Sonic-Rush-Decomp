#include <nitro/types.h>

typedef struct UnkStruct_022e237c {
    u8 pad[0x78];
    s32 unk78;
    s32 unk7C;
    u8 pad1[4];
    s32 unk84;
    s32 unk88;
    u8 pad2[2];
    u16 unk8E;
    u16 unk90;
} UnkStruct_022e237c;

typedef struct UnkStruct_022e237c_2 {
    u8 pad[0x18];
    u16 unk18;
    u16 unk1A;
} UnkStruct_022e237c_2;

extern void *gZoneActArchive;
extern u8 data_ov000_02365904[];
extern UnkStruct_022e237c data_ov000_0236a48c;
extern u32 data_ov000_02363738[];
extern s32 data_ov000_023637ec[][3];
extern s16 data_ov000_0236372c[][3];
extern u16 data_ov000_02363744[][3];

void NNS_FndMountArchive(void *ctx, void *a, void *b);
UnkStruct_022e237c_2 *NNS_FndGetArchiveFileByIndex(void *ctx, u32 id);
void Background_Init(void *buf, void *res, s32 a, s32 b, s32 c, s32 d, s32 e);
void Background_Draw(void *buf);
void NNS_FndUnmountArchive(void *ctx);

void Boss6_SetDefeatCameraShot(s32 a, s32 b) {
    UnkStruct_022e237c *p;
    u8 buf[0x40];
    u8 ctx[0x5c];
    UnkStruct_022e237c_2 *r;
    p = (UnkStruct_022e237c *)(u32)&data_ov000_0236a48c;
    NNS_FndMountArchive(ctx, data_ov000_02365904, gZoneActArchive);
    r = NNS_FndGetArchiveFileByIndex(ctx, data_ov000_02363738[b]);
    Background_Init(buf, r, 0x38, 0, 2, r->unk18, r->unk1A);
    Background_Draw(buf);
    NNS_FndUnmountArchive(ctx);
    p->unk78 = p->unk7C = data_ov000_023637ec[a][b];
    p->unk84 = p->unk88 = data_ov000_0236372c[a][b];
    p->unk8E = p->unk90 = data_ov000_02363744[a][b];
}
