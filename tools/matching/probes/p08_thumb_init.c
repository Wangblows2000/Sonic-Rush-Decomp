// target: main 0x02040ec0 f
#pragma thumb on
typedef unsigned long u32;
typedef struct { u32 a; u32 limit; } L;
typedef struct { char p0[0x10]; u32 flags; u32 v; u32 f18; u32 f1c; int f20; int f24; L *l; } S;
void f(S *s, u32 v) {
    if (v < s->l->limit) s->v = v;
    s->flags &= ~0x49; s->f18 = 0; s->f1c = 0x1000; s->f20 = -1; s->f24 = -1; s->flags |= 0x20;
}
