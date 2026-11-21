#include <nitro/types.h>

typedef struct UnkStruct_022c7b20 UnkStruct_022c7b20;
struct UnkStruct_022c7b20 {
    u8 pad[0x68];
    void (*func)(UnkStruct_022c7b20 *s);
    u8 pad1[0x20c - 0x6c];
    u16 unk20C;
    s16 unk20E;
    u16 unk210;
};

typedef struct UnkStruct_022c7b20_2 {
    u8 pad[0x21c];
    s16 unk21C;
    s16 unk21E;
    u8 pad1[0x22a - 0x220];
    u16 unk22A;
} UnkStruct_022c7b20_2;

extern UnkStruct_022c7b20_2 *gBoss1;
extern u16 data_ov000_02362b8c[];

void Boss1Neck_State_FireAtTarget(UnkStruct_022c7b20 *s);

void Boss1Neck_State_AimAtTarget(UnkStruct_022c7b20 *s) {
    s16 *p;
    s16 t;
    UnkStruct_022c7b20_2 *g = gBoss1;
    if (s->unk20C & 1)
        p = &g->unk21C;
    else
        p = &g->unk21E;
    t = s->unk20E;
    if (*p < t) {
        if (t - *p > 0x100)
            *p += 0x100;
        else
            *p = t;
    } else if (*p > t) {
        if (*p - t > 0x100)
            *p -= 0x100;
        else
            *p = t;
    }
    if (data_ov000_02362b8c[gBoss1->unk22A] < s->unk210++)
        s->func = Boss1Neck_State_FireAtTarget;
}
