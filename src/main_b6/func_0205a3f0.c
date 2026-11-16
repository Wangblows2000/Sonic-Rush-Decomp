#include <nitro/types.h>

typedef struct {
    u8 pad0[2];
    u8 b2;
    u8 b3;
    u32 arr4[7][3];
    u8 pad58[2];
    u8 b5a;
    u8 pad5b;
    u32 arr5c[7][3];
} UnkSave_0205a3f0;

typedef struct {
    s32 unk0;
    u8 pad4[0x18];
    u8 zone;
    u8 act;
    u8 pad1e[0xa];
    u16 unk28;
} UnkState_0205a3f0;

typedef struct {
    u32 score;
    u8 pad4[0x10];
    u8 b14;
} UnkState2_0205a3f0;

extern UnkSave_0205a3f0 data_022c468c;
extern UnkState_0205a3f0 data_022c4560;
extern UnkState2_0205a3f0 data_022c4590;

u32 func_0204e6e8(u8 zone);
u8 func_0204e6e4(u8 act);
void func_0205a4ec(void (*f)(void));

void func_0205a3f0(void) {
    UnkSave_0205a3f0 *s = &data_022c468c;
    u32 z = func_0204e6e8(data_022c4560.zone);
    u8 a = func_0204e6e4(data_022c4560.act);

    if (data_022c4560.unk0 != 1) {
        s->b2 = data_022c4590.b14;
        s->b3 = data_022c4560.unk28 & 0x7f;
        if (data_022c4560.zone < 7) {
            u32 *row = s->arr4[z];
            u32 v = row[a];
            u32 t = data_022c4590.score;
            if (t >= v) {
                if (t > 999999)
                    t = 999999;
                v = t;
            }
            row[a] = v;
        }
    } else {
        s->b5a = data_022c4590.b14;
        if (data_022c4560.zone < 7) {
            u32 *row = s->arr5c[z];
            u32 v = row[a];
            u32 t = data_022c4590.score;
            if (t >= v) {
                if (t > 999999)
                    t = 999999;
                v = t;
            }
            row[a] = v;
        }
    }
    func_0205a4ec(NULL);
}
