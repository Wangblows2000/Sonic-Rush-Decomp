#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    u16 unk4;
} UnkRes_0203fdb0;

extern s32 data_022c196c;
extern s32 data_022c1980;
extern u16 data_022c1948;
extern u16 data_022c1950;
extern u8 data_022c2440[];
extern void (*data_022c197c)(const char *fmt, ...);
extern char data_02078a18[];
extern char data_02078a40[];

void func_020405bc(s32 r);
void func_020405e0(s32 a);
BOOL func_0203fd64(void);
s32 func_02012a58(void *buf, s32 a, u16 mask, u16 b, s32 c);
void func_0204045c(UnkRes_0203fdb0 *r);

void func_0203fdb0(UnkRes_0203fdb0 *r) {
    s32 ret;

    if (r->unk2 != 0) {
        func_020405bc(r->unk2);
        func_020405e0(9);
        return;
    }
    switch (r->unk4) {
    case 10:
        if (data_022c196c == 2) {
            if (data_022c1980 == 4) {
                if (func_0203fd64())
                    return;
                if (data_022c197c != NULL)
                    data_022c197c(data_02078a18);
                func_020405e0(9);
                return;
            }
            if (data_022c1980 == 6)
                return;
        } else if (data_022c196c == 4) {
            ret = func_02012a58(data_022c2440, 0xd, (1 << (data_022c1948 + 1)) - 1, data_022c1950, 1);
            if (ret != 0) {
                func_020405bc(ret);
                func_020405e0(9);
                return;
            }
            func_020405e0(5);
            return;
        } else if (data_022c196c == 6) {
            func_0204045c(r);
        } else if (data_022c196c == 7) {
            func_0204045c(r);
        }
        func_020405e0(4);
        return;
    case 11:
        break;
    case 12:
    case 13:
    default:
        if (data_022c197c != NULL)
            data_022c197c(data_02078a40, r->unk4);
        break;
    }
}
