#include <nitro/types.h>

void TransformDemo_InitDisplay(void *p);
void TransformDemo_LoadPlayerArchive(void *p, void *q);
void func_ov001_022df440(void *p, void *q);
void func_ov001_022df950(void *p);
void func_ov001_022dfc54(void *p, void *q);
void func_ov001_022df490(void *p);
void func_ov001_022e034c(void);

void TransformDemo_State_Init(u8 *p) {
    TransformDemo_InitDisplay(p);
    TransformDemo_LoadPlayerArchive(p, p + 0x454);
    func_ov001_022df440(p, p + 0xbd4);
    func_ov001_022df950(p);
    func_ov001_022dfc54(p, p + 0x14);
    func_ov001_022df490(p);
    *(u16 *)(p + 0xc) = 0x1e;
    *(void (**)(void))p = func_ov001_022e034c;
}
