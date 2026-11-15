#include <nitro/types.h>

typedef struct { u8 data[0x5c]; } UnkStruct_023029d4;
extern u8 data_ov001_0231d9e4[];
void func_02021310(UnkStruct_023029d4 *a, void *b, void *c);
void *func_02021264(UnkStruct_023029d4 *a, s32 i);
void func_020212e0(UnkStruct_023029d4 *a);
void *func_02025e8c(s32 a);
s32 func_02037f94(void *a, s32 b);
void func_02035704(void *p, s32 a, s32 b, s32 c);

void func_ov001_022ffb9c(s32 a, void *b, s32 *c, u16 *d, u16 *e, s32 n) {
    UnkStruct_023029d4 arc;
    void *h;
    func_02021310(&arc, data_ov001_0231d9e4, b);
    h = func_02025e8c(a);
    while (n-- != 0) {
        void *res = func_02021264(&arc, *d);
        func_02035704(res, *e, 5, func_02037f94(h, *c));
        c++;
        d++;
        e++;
    }
    func_020212e0(&arc);
}
