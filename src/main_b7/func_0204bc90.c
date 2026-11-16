#include <nitro/types.h>

typedef struct {
    u32 flags;
    void (*cb)(void);
} UnkWork_0204bc90;

typedef struct {
    u32 unk0;
    u32 unk4;
    void (*fn)(void);
    u32 unkc;
    UnkWork_0204bc90 *work;
} UnkTask_0204bc90;

typedef struct {
    u8 pad0[0x10];
    s32 unk10;
    u8 pad14[4];
    u32 flags;
} UnkStruct_022c4560;

typedef struct {
    u8 pad0[0x24];
    u32 flags;
} UnkStruct_022c4590;

extern UnkTask_0204bc90 *data_022b4574;
extern UnkStruct_022c4560 data_022c4560;
extern UnkStruct_022c4590 data_022c4590;
void func_0204bf54(void);
void func_0204ca68(void);
void func_0204c964(void);
void func_0204cb10(void);
void func_0204b3b8(void);
void func_0204b414(void);
void func_0204b528(void);
void func_0204b624(void);
void func_02039b48(void);
void func_020395f4(void);
void func_02036894(void);
void func_02033754(void);
void func_02035cb4(void);
void func_02035368(void);
void func_020371c8(UnkTask_0204bc90 *t);
void func_02037d40(void);

void func_0204bc90(void) {
    UnkWork_0204bc90 *w = data_022b4574->work;
    void (*cb)(void) = w->cb;
    u32 flags = w->flags;
    func_0204bf54();
    if (data_022c4560.unk10 == 0) {
        if (data_022c4560.flags & 0x40) {
            func_0204ca68();
            func_0204c964();
        } else {
            func_0204cb10();
        }
    }
    func_0204b3b8();
    func_0204b414();
    if (flags & 2)
        func_0204b528();
    if (flags & 1)
        func_0204b624();
    func_02039b48();
    func_020395f4();
    func_02036894();
    func_02033754();
    func_02035cb4();
    func_02035368();
    func_020371c8(data_022b4574);
    if (!(data_022c4560.flags & 0x40))
        data_022c4590.flags &= ~2;
    func_02037d40();
    if (cb != NULL)
        cb();
}
