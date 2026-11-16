#include <nitro/types.h>

typedef struct {
    void (*func)(void *);
    u32 unk4;
    s32 state;
    u8 padc[0xb];
    u8 unk17;
    u16 unk18;
    u16 unk1a;
} UnkObj_0204211c;

typedef struct {
    u16 unk0;
    u16 unk2;
} UnkStruct_0224ffa0;

typedef struct {
    u8 pad[8];
    void (*func)(void);
} UnkTask_022b4574;

extern UnkStruct_0224ffa0 data_0224ffa0;
extern UnkTask_022b4574 *data_022b4574;
u16 func_0203e944(void);
u32 func_02042998(void);
void func_020420fc(void *);
void func_02042684(void);

void func_0204211c(UnkObj_0204211c *o) {
    UnkStruct_0224ffa0 *s = (UnkStruct_0224ffa0 *)((u32)&data_0224ffa0 | 0);
    s->unk0 = func_0203e944();
    if (func_02042998() > 1) {
        o->state = 2;
        if (o->unk17 != 0) {
            s->unk2 |= 1;
            o->unk18 = 0;
            o->func = func_020420fc;
            data_022b4574->func = func_02042684;
        }
    } else {
        o->state = 1;
    }
    o->unk17 = 0;
    o->unk1a = 0;
}
