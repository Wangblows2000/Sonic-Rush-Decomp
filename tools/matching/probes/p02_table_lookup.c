// target: main 0x02021264 f
typedef unsigned long u32; typedef unsigned short u16;
typedef struct { u32 off; u32 x; } E;
typedef struct { char pad[8]; u16 count; u16 pad2; E e[1]; } H;
typedef struct { char pad[0x54]; H *h; u32 base; } X;
u32 f(X *x, u32 idx) { H *h = x->h; u32 r = 0; if (idx < h->count) { r = x->base + h->e[idx].off; } return r; }
