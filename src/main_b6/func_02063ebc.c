#include <nitro/types.h>

typedef struct { u8 pad[0x34]; s32 v; u8 pad2[0x10]; } Ent;
typedef struct { u8 pad[8]; s32 h[2]; Ent e[25]; } Work;
typedef struct { u8 pad[0x10]; Work *w; } Obj;
extern s32 data_022c4dc4;
void func_02039bb0(s32 a, s32 b);
void func_020371c8(s32 a);

void func_02063ebc(Obj *o) {
    Work *w = o->w;
    s16 i;
    Ent *e = w->e;
    for (i = 0; i < 25; i++, e++)
        func_02039bb0(1, e->v);
    for (i = 0; i < 2; i++)
        func_020371c8(w->h[i]);
    data_022c4dc4 = 0;
}
