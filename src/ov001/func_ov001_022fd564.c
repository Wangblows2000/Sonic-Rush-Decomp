#include <nitro/types.h>

typedef struct { u8 data[0x5c]; } UnkStruct_023029d4;
typedef struct { u32 flags; s32 ids[6]; } UnkStruct_0231d6b4;
typedef struct { void *res[6]; } UnkStruct_0231f098;
extern void *data_ov001_0231f528;
extern u8 data_ov001_0231d9a8[];
extern UnkStruct_0231d6b4 data_ov001_0231d6b4[];
extern UnkStruct_0231f098 data_ov001_0231f098[];
void func_02021310(UnkStruct_023029d4 *a, void *b, void *c);
void *func_02021264(UnkStruct_023029d4 *a, s32 i);
void func_020212e0(UnkStruct_023029d4 *a);
void func_020257f8(void *p);

void func_ov001_022fd564(void) {
    UnkStruct_023029d4 arc;
    s32 i, j;
    UnkStruct_0231d6b4 *d;
    UnkStruct_0231f098 *r;
    func_02021310(&arc, data_ov001_0231d9a8, data_ov001_0231f528);
    d = data_ov001_0231d6b4;
    r = data_ov001_0231f098;
    for (i = 0; i < 0x1b; i++) {
        for (j = 0; j < 6; j++) {
            if (d->ids[j] != -1) {
                r->res[j] = func_02021264(&arc, d->ids[j]);
            }
        }
        if (d->flags & 1) {
            func_020257f8(r->res[0]);
        }
        d++;
        r++;
    }
    func_020212e0(&arc);
}
