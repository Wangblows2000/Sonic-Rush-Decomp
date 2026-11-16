#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
} UnkRes_0203ee8c;

extern s32 data_022c196c;
extern s32 data_022c1980;
extern u16 data_022c1948;
extern u16 data_022c1950;
extern u8 data_022c2440[];
extern void (*data_022c197c)(const char *fmt, ...);
extern char data_020787d4[];
extern char data_020787fc[];
extern char data_02078830[];

void func_020405bc(s32 r);
void func_020405e0(s32 a);
void func_0203dda4(void);
void func_0203dc28(void);
BOOL func_0203ee0c(void);
s32 func_02012a58(void *buf, s32 a, u16 mask, u16 b, s32 c);
void func_0204045c(UnkRes_0203ee8c *r);

void func_0203ee8c(UnkRes_0203ee8c *r) {
    s32 ret;

    if (r->unk2 != 0) {
        if (r->unk2 == 0xf)
            return;
        if (r->unk2 == 9)
            return;
        if (r->unk2 == 0xd)
            return;
        func_020405bc(r->unk2);
        func_0203dda4();
        return;
    }
    switch (r->unk4) {
    case 10:
        if (data_022c196c == 3) {
            if (data_022c1980 == 6)
                return;
            if (data_022c1980 == 4) {
                if (func_0203ee0c())
                    return;
                if (data_022c197c != NULL)
                    data_022c197c(data_020787d4);
                func_0203dc28();
                return;
            }
        } else if (data_022c196c == 5) {
            ret = func_02012a58(data_022c2440, 0xd, (1 << (data_022c1948 + 1)) - 1, data_022c1950, 1);
            if (ret != 0) {
                func_020405bc(ret);
                func_0203dc28();
                return;
            }
            if (data_022c197c != NULL)
                data_022c197c(data_020787fc);
            func_020405e0(5);
            return;
        } else if (data_022c196c == 7) {
            func_0204045c(r);
        }
        func_020405e0(4);
        return;
    case 12:
    case 13:
        break;
    case 11:
    default:
        if (data_022c197c != NULL)
            data_022c197c(data_02078830, r->unk4);
        break;
    }
}
