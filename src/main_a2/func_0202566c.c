#include <nitro/types.h>

extern void (*data_02077d4c)(u32 key);
extern void (*data_02077d44)(u32 key);

void *func_02025ecc(u32 *res);
void *func_02025e8c(u32 *res);
void func_020214f0(void *mdlSet);
u32 func_02021a44(void *tex);
void func_02021ae4(void *tex, u32 *k1, u32 *k2);

void func_0202566c(u32 *res) {
    switch (*res) {
    case 0x30444D42: {
        void *mdlSet = func_02025ecc(res);
        if (func_02025e8c(res) != NULL) func_020214f0(mdlSet);
    }
    case 0x30585442: {
        void *tex = func_02025e8c(res);
        if (tex != NULL) {
            u32 texKey = func_02021a44(tex);
            u32 k1;
            u32 k2;
            func_02021ae4(tex, &k1, &k2);
            if (texKey != 0) data_02077d4c(texKey);
            if (k2 != 0) data_02077d44(k2);
            if (k1 != 0) data_02077d44(k1);
        }
        break;
    }
    case 0x30415642:
    case 0x30414D42:
    case 0x30414342:
    case 0x30415442:
    case 0x30505442:
        break;
    }
}
