#include <nitro/types.h>

void func_02025454(void);
BOOL func_020040e4(void *m);
void func_02001ce4(void *m, void *dst);
BOOL func_0200409c(void *p);

#define REG_MTX_MODE (*(vu32 *)0x04000440)
#define REG_MTX_PUSH (*(vu32 *)0x04000444)
#define REG_MTX_POP (*(vu32 *)0x04000448)
#define REG_MTX_IDENTITY (*(vu32 *)0x04000454)

void func_02025a94(void *mtx, void *p) {
    u8 buf[0x40];
    func_02025454();
    REG_MTX_MODE = 0;
    REG_MTX_PUSH = 0;
    REG_MTX_IDENTITY = 0;
    if (mtx != NULL) {
        while (func_020040e4(buf)) {
        }
        func_02001ce4(buf, mtx);
    }
    if (p != NULL) {
        while (func_0200409c(p)) {
        }
    }
    REG_MTX_POP = 1;
    REG_MTX_MODE = 2;
}
