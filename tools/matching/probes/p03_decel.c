// target: main 0x02068db4 f
extern short g;
int f(int a, int b) {
    if (a > 0) { a -= (b * g) >> 8; if (a < 0) a = 0; }
    else { a += (b * g) >> 8; if (a > 0) a = 0; }
    return a;
}
