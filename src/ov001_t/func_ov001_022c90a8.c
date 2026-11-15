#include <nitro/types.h>

extern char *data_ov001_0231f048;

typedef struct {
    u8 pad[0x38];
    s32 text[2][0x29];
} UnkStruct_022c90a8;

#pragma thumb on
void func_ov001_022c90a8(UnkStruct_022c90a8 *p, s32 a, s32 b) {
    char c;
    char *s;
    s32 i;

    i = 0;
    if (a >= 0) {
        s = data_ov001_0231f048 + a * 0x30;
        for (; i < 0x29; i++) {
            if (i == 0) {
                p->text[0][i] = 0x28;
                continue;
            }
            c = s[i - 1];
            if (c == 0) break;
            if (c == ' ') {
                p->text[0][i] = 0;
            } else if (c >= 'A' && c <= 'Z') {
                p->text[0][i] = c - 0x40;
            } else if (c >= '0' && c <= '9') {
                p->text[0][i] = c - 0x15;
            } else if (c == '(') {
                p->text[0][i] = 0x25;
            } else if (c == ')') {
                p->text[0][i] = 0x26;
            } else if (c == '-') {
                p->text[0][i] = 0x27;
            } else if (c == ',') {
                p->text[0][i] = 0x29;
            } else if (c == '\'') {
                p->text[0][i] = 0x2a;
            } else if (c == '#') {
                p->text[0][i] = 0x2b;
            } else if (c == '!') {
                p->text[0][i] = 0x2c;
            } else {
                p->text[0][i] = 0;
            }
        }
    }
    for (; i < 0x29; i++) {
        p->text[0][i] = 0x2e;
    }

    i = 0;
    if (b >= 0) {
        s = data_ov001_0231f048 + b * 0x30;
        for (; i < 0x29; i++) {
            if (i == 0) {
                p->text[1][i] = 0x28;
                continue;
            }
            c = s[i - 1];
            if (c == 0) break;
            if (c == ' ') {
                p->text[1][i] = 0;
            } else if (c >= 'A' && c <= 'Z') {
                p->text[1][i] = c - 0x40;
            } else if (c >= '0' && c <= '9') {
                p->text[1][i] = c - 0x15;
            } else if (c == '(') {
                p->text[1][i] = 0x25;
            } else if (c == ')') {
                p->text[1][i] = 0x26;
            } else if (c == '-') {
                p->text[1][i] = 0x27;
            } else if (c == ',') {
                p->text[1][i] = 0x29;
            } else if (c == '\'') {
                p->text[1][i] = 0x2a;
            } else if (c == '#') {
                p->text[1][i] = 0x2b;
            } else if (c == '!') {
                p->text[1][i] = 0x2c;
            } else {
                p->text[1][i] = 0;
            }
        }
    }
    for (; i < 0x29; i++) {
        p->text[1][i] = 0x2e;
    }
}
#pragma thumb reset
