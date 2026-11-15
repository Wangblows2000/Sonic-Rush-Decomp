// target: main 0x02033550 f
typedef unsigned long u32; typedef unsigned short u16;
typedef struct { u32 a; u32 b; } E;
typedef struct { u32 x; u32 off; } T;
typedef struct { char p0[8]; u16 f8; u16 fa; u16 fc; u16 fe; T *t; char p14[8]; u32 f1c; char p20[8]; u16 f28; u16 p2a; u32 f2c; } S;
void f(S *s, u16 v) {
    s->f8 = v; s->fa = 0; s->f28 = 0; s->fe = 0; s->fc = 0;
    s->f2c &= ~3;
    s->f1c = ((E *)((char *)s->t + s->t->off))[v].b;
}
