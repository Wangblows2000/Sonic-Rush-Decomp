#include <nitro/types.h>

typedef struct {
    u16 unk0;
    u16 unk2;
    u8 pad4[4];
    u16 unk8;
    u8 pada[6];
    u16 unk10;
} UnkRes_02040048;

extern void (*data_022c197c)(const char *fmt, ...);
extern BOOL (*data_022c1978)(UnkRes_02040048 *r);
extern s32 data_022c196c;
extern u16 data_022c1944;
extern char data_02078a60[];
extern char data_02078a8c[];
extern char data_02078ab8[];

void func_020405bc(s32 r);
void func_020405e0(s32 a);
s32 func_02011598(s32 a, u16 b);
void func_0204045c(UnkRes_02040048 *r);
BOOL func_0203ff68(void);

void func_02040048(UnkRes_02040048 *r) {
    u16 bit = 1 << r->unk10;
    s32 ret;

    if (r->unk2 != 0) {
        func_020405bc(r->unk2);
        func_020405e0(9);
        return;
    }
    switch (r->unk8) {
    case 7:
        if (data_022c197c != NULL)
            data_022c197c(data_02078a60, r->unk10);
        if (data_022c1978 != NULL && !data_022c1978(r)) {
            ret = func_02011598(0, r->unk10);
            if (ret == 2)
                return;
            func_020405bc(ret);
            func_020405e0(9);
            return;
        }
        if (data_022c196c == 6 || data_022c196c == 7)
            func_0204045c(r);
        data_022c1944 |= bit;
        return;
    case 9:
        if (data_022c197c != NULL)
            data_022c197c(data_02078a8c, r->unk10);
        data_022c1944 &= ~bit;
        func_0204045c(r);
        return;
    case 0:
        if (!func_0203ff68())
            func_020405e0(9);
        return;
    case 2:
        return;
    default:
        if (data_022c197c != NULL)
            data_022c197c(data_02078ab8, r->unk8);
        return;
    }
}
