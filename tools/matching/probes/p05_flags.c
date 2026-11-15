// target: main 0x0206e2b4 f
typedef unsigned short u16;
typedef struct { char pad[0xa]; u16 flags; char pad2[0x14]; int a; int b; } S;
void f(S *s, int a, int b) { s->a = a; s->b = b; s->flags &= ~0x200; s->flags &= ~0x400; }
