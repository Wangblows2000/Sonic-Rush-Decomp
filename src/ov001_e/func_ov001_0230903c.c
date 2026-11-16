#include <nitro/types.h>

typedef struct UnkStruct_0230903c_o {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x48 - 8];
} UnkStruct_0230903c_o;

typedef struct UnkStruct_0230903c {
    u8 pad0[0x780];
    UnkStruct_0230903c_o objA;
    UnkStruct_0230903c_o objB;
    u8 pad810[0xae4 - 0x810];
    s32 unkAE4;
    u8 padAE8[0xba8 - 0xae8];
    s32 unkBA8;
    s32 unkBAC;
    u8 padBB0[4];
    s32 unkBB4;
    u8 padBB8[0xbc8 - 0xbb8];
    s32 timer;
    u8 padBCC[4];
    s32 unkBD0;
} UnkStruct_0230903c;

typedef struct UnkStruct_0230903c_t {
    u8 pad0[0x148];
    s32 unk148;
} UnkStruct_0230903c_t;

extern UnkStruct_0230903c *data_ov001_0231f5bc;

void func_ov001_023085f4(s32 t);
void func_ov001_0230832c(s32 t);
u16 func_ov001_02314380(void);
UnkStruct_0230903c_t *func_ov001_0230c0a4(void);
void AnimatorSprite_SetAnimation(UnkStruct_0230903c_o *s, u16 index);
void func_ov001_0231599c(s32 v);
void func_ov001_02312e04(void *p);
s32 func_ov001_0230c0cc(void);
u16 func_ov001_02314368(void);

void func_ov001_0230903c(void) {
    UnkStruct_0230903c *g = data_ov001_0231f5bc;
    s32 n;

    g->timer++;
    func_ov001_023085f4(g->timer - 1);
    func_ov001_0230832c(g->timer - 0xb4);
    switch (g->timer) {
    case 1:
        g->objB.x = 0x80;
        g->objB.y = 0x2a;
        n = func_ov001_02314380();
        if (n <= func_ov001_0230c0a4()->unk148) {
            AnimatorSprite_SetAnimation(&g->objB, 3);
            func_ov001_0231599c(4);
            func_ov001_02312e04(func_ov001_0230c0a4());
            return;
        }
        AnimatorSprite_SetAnimation(&g->objB, 10);
        g->timer = -1;
        g->unkBD0 = 0;
        return;
    case 0xf0:
        g->objA.x = 0x80;
        g->objA.y = 0x22;
        AnimatorSprite_SetAnimation(&g->objA, func_ov001_0230c0cc() * 2 + 2);
        g->unkBB4 = 1;
        g->unkBA8 = 1;
        g->unkBAC = 0;
        return;
    case 0x172:
        g->timer = -1;
        func_ov001_0231599c(0);
        break;
    default:
        return;
    }
    g->unkAE4 = func_ov001_02314368();
}
