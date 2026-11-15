#include <nitro/types.h>

typedef struct {
    s16 x;
    s16 y;
    u8 pad[0x28];
} UnkStruct_02040d14;

typedef struct {
    s32 x;
    s32 y;
    s16 w;
    s16 h;
    s32 type;
} UnkRect_022e432c;

typedef struct {
    u8 pad[0xf94];
    UnkRect_022e432c rects[7];
} UnkStruct_022e432c;

void func_02040ef0(void *p, s32 a, s32 b);
void func_02040ec0(void *p, u32 value);
void func_02040d14(void *p);

#define SET_RECT(i, anim, ww, hh, t)     \
    r = &p->rects[i];                    \
    func_02040ec0(&buf, anim);           \
    func_02040d14(&buf);                 \
    r->x = buf.x;                        \
    r->y = buf.y;                        \
    r->w = ww;                           \
    r->h = hh;                           \
    r->type = t;

#pragma thumb on
void func_ov001_022e432c(UnkStruct_022e432c *p, s32 a) {
    UnkStruct_02040d14 buf;
    UnkRect_022e432c *r;
    func_02040ef0(&buf, a, 0);
    SET_RECT(0, 0x39, 0x38, 0x18, 0)
    SET_RECT(1, 0x3a, 0x38, 0x18, 0)
    SET_RECT(2, 0x3b, 0x38, 0x18, 0)
    SET_RECT(3, 0x3c, 0x38, 0x18, 0)
    SET_RECT(4, 0x3d, 0x88, 0x40, 1)
    SET_RECT(5, 0x3e, 0x30, 0x10, 2)
    SET_RECT(6, 0x3f, 0x30, 0x10, 2)
}
#pragma thumb reset
