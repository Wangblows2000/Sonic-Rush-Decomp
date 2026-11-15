// target: main 0x0201bf1c f
void f(int **a) { int *p = *a; if (p != 0) { *p = 0; *a = 0; } }
