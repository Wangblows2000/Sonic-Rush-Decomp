#include <nitro/types.h>

typedef struct UnkStruct_022cbcf4 {
    u8 pad[0x10];
    u8 *unk10;
} UnkStruct_022cbcf4;

extern void *gZoneActArchive;
extern u8 data_ov000_02364fbc[];
extern u8 data_ov000_02364fc0[];
extern u8 data_ov000_02364fd4[];
extern u8 data_ov000_02364ff0[];
extern u8 data_ov000_0236500c[];
extern u8 data_ov000_02365024[];
extern u8 data_ov000_0236503c[];

void func_02021310(void *ctx, void *a, void *b);
void func_020212e0(void *ctx);
void *func_0202128c(void *a);
void func_0202566c(void *a);
BOOL func_ov000_022cc6d4(void);
void Model_Release(void *p);
void func_ov000_022fb9c8(UnkStruct_022cbcf4 *s);

void func_ov000_022cbcf4(UnkStruct_022cbcf4 *s) {
    u8 ctx[0x5c];
    u8 *t = s->unk10;
    func_02021310(ctx, data_ov000_02364fbc, gZoneActArchive);
    func_0202566c(func_0202128c(data_ov000_02364fc0));
    func_0202566c(func_0202128c(data_ov000_02364fd4));
    func_0202566c(func_0202128c(data_ov000_02364ff0));
    if (func_ov000_022cc6d4()) {
        func_0202566c(func_0202128c(data_ov000_0236500c));
        func_0202566c(func_0202128c(data_ov000_02365024));
        func_0202566c(func_0202128c(data_ov000_0236503c));
    }
    func_020212e0(ctx);
    Model_Release(t + 0x23c);
    func_ov000_022fb9c8(s);
}
