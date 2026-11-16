#include <nitro/types.h>

typedef struct UnkStruct_0230b164_br {
    u8 pad0[4];
    s16 x;
    s16 y;
    u8 pad8[0x20 - 8];
    u16 b0 : 1;
    u16 p1 : 5;
    u16 b6 : 2;
    u16 p8 : 1;
    u16 b9 : 1;
    u16 b10 : 1;
    u16 p11 : 1;
    u16 b12 : 1;
    u16 p13 : 3;
    u16 e0 : 5;
    u16 p5 : 3;
    u16 e8 : 5;
    u16 p13b : 3;
} UnkStruct_0230b164_br;

typedef struct UnkStruct_0230b164_t {
    u8 pad0[8];
    void (*func)(void);
} UnkStruct_0230b164_t;

typedef struct UnkStruct_0230b164_o {
    u8 pad0[0x34];
    u32 unk34;
    u8 pad38[0x48 - 0x38];
} UnkStruct_0230b164_o;

typedef struct UnkStruct_0230b164 {
    u8 pad0[0xc];
    UnkStruct_0230b164_t *taskC;
    UnkStruct_0230b164_t *task10;
    void *task14;
    s32 unk18;
    u8 pad1C[0xf8 - 0x1c];
    UnkStruct_0230b164_o objs[3];
} UnkStruct_0230b164;

typedef struct UnkStruct_0230b164_e {
    u32 a;
    u32 b;
    u32 c;
} UnkStruct_0230b164_e;

extern UnkStruct_0230b164_br data_0208922c;
extern UnkStruct_0230b164 *data_ov001_0231f5c0;
extern UnkStruct_0230b164_e data_ov001_023171c8[];

void func_020371c8(void *task);
void func_02039bb0(u32 a, u32 b);
void func_ov001_02312e74(s32 a);
void func_ov001_023099ec(void);
void func_020367b4(s32 a, s32 b);
void func_ov001_0230bbd8(void);
void func_ov001_0230ba9c(void);

void func_ov001_0230b164(void) {
    UnkStruct_0230b164 *g;
    s32 i;

    g = data_ov001_0231f5c0;
    data_0208922c.b0 = 0;
    data_0208922c.x = 0;
    data_0208922c.b9 = 0;
    data_0208922c.b10 = 0;
    data_0208922c.b12 = 0;
    data_0208922c.y = 0;
    data_0208922c.b6 = 0;
    data_0208922c.e0 = 0;
    data_0208922c.e8 = 0;
    func_020371c8(g->task14);
    g->task14 = NULL;
    for (i = 0; i < 3; i++)
        func_02039bb0(data_ov001_023171c8[i].a, g->objs[i].unk34);
    func_ov001_02312e74(g->unk18);
    func_ov001_023099ec();
    func_020367b4(0, 0x1e);
    g->taskC->func = func_ov001_0230bbd8;
    g->task10->func = func_ov001_0230ba9c;
}
