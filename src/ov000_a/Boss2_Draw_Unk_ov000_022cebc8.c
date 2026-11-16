#include <nitro/types.h>

typedef struct UnkStruct_022cebc8 {
    u8 pad[0x10];
    s32 unk10;
    s32 unk14;
    u8 pad1[0x5c - 0x18];
    u16 unk5C;
    u8 pad2[0x158 - 0x5e];
    u8 unk158[0x17c - 0x158];
    s32 unk17C;
    s32 unk180;
    s32 unk184;
} UnkStruct_022cebc8;

typedef struct UnkStruct_022cebc8_g {
    u8 pad[0x20c];
    u32 unk20C;
} UnkStruct_022cebc8_g;

typedef struct UnkStruct_022cebc8_cam {
    u8 pad[0x70];
    s32 unk70;
    u8 pad1[0xd6 - 0x74];
    u16 unkD6;
} UnkStruct_022cebc8_cam;

typedef struct UnkStruct_022cebc8_2 {
    u8 pad[0x62];
    u16 unk62;
} UnkStruct_022cebc8_2;

typedef struct UnkStruct_022cebc8_v {
    s16 x;
    s16 y;
    s16 z;
} UnkStruct_022cebc8_v;

typedef struct UnkStruct_022cebc8_4 {
    u8 pad[0x1c];
    u8 unk1C;
    u8 unk1D;
} UnkStruct_022cebc8_4;

extern UnkStruct_022cebc8 *gPlayers;
extern UnkStruct_022cebc8_g *data_ov000_02369ae0;
extern UnkStruct_022cebc8_cam data_ov000_0236a444;
extern UnkStruct_022cebc8_2 data_ov000_0236aad8;
extern UnkStruct_022cebc8_4 data_022c4560;
extern s16 data_0206e470[];
extern u16 data_ov000_02362df4[];
extern UnkStruct_022cebc8_v data_ov000_02362ef8[];
extern u16 data_02074960[][3][3];
extern UnkStruct_022cebc8_v data_020748bc[][3];

void func_ov000_022c5e80(s32 len, s32 b, s32 a, s32 *outX, s32 *outY, u16 *outAng);
void MTX_RotY33_(void *m, s32 s, s32 c);
void func_02022270(s32 i, u16 a);
void func_02022284(s32 i, s32 x, s32 y, s32 z);

void Boss2_Draw_Unk_ov000_022cebc8(void) {
    UnkStruct_022cebc8 *p = gPlayers;
    u16 ang;
    u16 off;
    s32 i;
    if (p->unk5C & 0x20)
        return;
    if (p->unk10 < 0x4000)
        p->unk10 += 0x6cd98;
    else if (p->unk10 > 0x70d98)
        p->unk10 -= 0x6cd98;
    func_ov000_022c5e80(0x1152f0, 0x6cd988, p->unk10 << 4, &p->unk17C, &p->unk184, &ang);
    p->unk180 = -p->unk14 << 4;
    if (p->unk5C & 1)
        off = 0xc000;
    else
        off = 0x4000;
    MTX_RotY33_(p->unk158, data_0206e470[(((u16)(off + ang) & 0xffff) >> 4) * 2], data_0206e470[(((u16)(off + ang) & 0xffff) >> 4) * 2 + 1]);
    if (data_ov000_02369ae0->unk20C & 0x20)
        data_ov000_0236a444.unkD6 = ang;
    if ((data_ov000_0236aad8.unk62 << 12) > -data_ov000_0236a444.unk70) {
        for (i = 0; i <= 2; i++) {
            func_02022270(i, data_ov000_02362df4[i]);
            func_02022284(i, data_ov000_02362ef8[i].x, data_ov000_02362ef8[i].y, data_ov000_02362ef8[i].z);
        }
        return;
    }
    for (i = 0; i <= 2; i++) {
        func_02022270(i, data_02074960[data_022c4560.unk1C][data_022c4560.unk1D][i]);
        func_02022284(i, data_020748bc[data_022c4560.unk1C][i].x, data_020748bc[data_022c4560.unk1C][i].y, data_020748bc[data_022c4560.unk1C][i].z);
    }
}
