#include <nitro/types.h>

typedef struct UnkStruct_022c767c UnkStruct_022c767c;
struct UnkStruct_022c767c {
    u8 pad[0x68];
    void (*func)(UnkStruct_022c767c *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
};

typedef struct UnkStruct_022c767c_2 {
    u8 pad[0x22a];
    u16 unk22A;
} UnkStruct_022c767c_2;

typedef struct UnkStruct_022c767c_3 {
    u8 pad[0x10];
    s32 unk10;
} UnkStruct_022c767c_3;

typedef struct UnkStruct_022c767c_v {
    s32 x;
    s32 y;
    s32 z;
} UnkStruct_022c767c_v;

extern u32 gRandSeed;
extern UnkStruct_022c767c_2 *data_ov000_02369acc;
extern UnkStruct_022c767c_3 *gPlayers;
extern s16 data_ov000_02362b94[];
extern u16 data_ov000_02362b84[];

void *MapObject_Spawn(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f, s32 g, s32 h, s32 i);
void func_ov000_022cc530(UnkStruct_022c767c_v *v, s32 a);
void func_ov000_022ca484(void *obj, s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 d);
void BossEffect_CreateLaunch(s32 a, s32 x, s32 y, s32 z);
void Sound_PlaySeqArc(s32 a, s32 b, s32 c);
void func_ov000_022c7630(UnkStruct_022c767c *s);
void func_ov000_022c7970(UnkStruct_022c767c *s);

static inline u16 Rand(void) {
    gRandSeed = gRandSeed * 0x196225 + 0x3c6ef35f;
    return (u16)(gRandSeed >> 16);
}

void func_ov000_022c767c(UnkStruct_022c767c *s) {
    void *obj;
    UnkStruct_022c767c_v v;
    s32 x;
    obj = MapObject_Spawn(0xd3, 0, 0, 0, 0, 0, 0, 0, 0);
    func_ov000_022cc530(&v, s->unk20C & 1);
    x = gPlayers->unk10 << 4;
    x += ((Rand() & 0x7f) - 0x40) << 12;
    if (x < 0)
        x = 0;
    else if (x > 0x1c2000)
        x = 0x1c2000;
    func_ov000_022ca484(obj, v.x, v.y, v.z, x, -0x200000, 0, data_ov000_02362b94[data_ov000_02369acc->unk22A]);
    BossEffect_CreateLaunch(1, v.x >> 4, -v.y >> 4, v.z >> 4);
    Sound_PlaySeqArc(0, 0, 0x9c);
    s->unk20C++;
    if (data_ov000_02362b84[data_ov000_02369acc->unk22A] <= s->unk20C)
        s->func = func_ov000_022c7630;
    else
        s->func = func_ov000_022c7970;
}
