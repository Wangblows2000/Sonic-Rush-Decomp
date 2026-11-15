#include <nitro/types.h>

void func_ov001_022dffcc(void *p);
void func_ov001_022de388(void *p, void *q);
void func_ov001_022df440(void *p, void *q);
void func_ov001_022df950(void *p);
void func_ov001_022dfc54(void *p, void *q);
void func_ov001_022df490(void *p);
void func_ov001_022e034c(void);

void func_ov001_022e0450(u8 *p) {
    func_ov001_022dffcc(p);
    func_ov001_022de388(p, p + 0x454);
    func_ov001_022df440(p, p + 0xbd4);
    func_ov001_022df950(p);
    func_ov001_022dfc54(p, p + 0x14);
    func_ov001_022df490(p);
    *(u16 *)(p + 0xc) = 0x1e;
    *(void (**)(void))p = func_ov001_022e034c;
}
