// target: main 0x02025e8c f
typedef unsigned long u32; typedef unsigned short u16;
typedef struct { u32 magic; u32 a; u32 b; u16 off; u16 n; } H;
u32 f(H *h) {
    u32 *p = (u32 *)((char *)h + h->off);
    if (h->n == 1) { if (h->magic == 0x30585442) return (u32)h + p[0]; return 0; }
    return (u32)h + p[1];
}
