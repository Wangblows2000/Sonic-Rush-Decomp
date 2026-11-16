#include <nitro/types.h>

typedef struct {
    u8 pad0[0x68];
    void *unk68;
    u8 pad6C[0x434 - 0x6C];
    s32 unk434;
    u8 pad438[2];
    u8 unk43A[2];
    u16 unk43C;
    u16 unk43E;
} UnkStruct_022f3758;

void func_ov000_022f8fa0(void *p, s32 a, s32 b);
extern u32 data_022c4560;
void func_ov000_022f8c28(u32 a, s32 b);
void func_ov000_022f8bc8(u32 a, s32 b);
void func_ov000_022f4cbc(UnkStruct_022f3758 *s);
void func_ov000_022f369c(void);

void func_ov000_022f3758(UnkStruct_022f3758 *s) {
    func_ov000_022f8fa0(s->unk43A, 2, 6);
    s->unk43E++;
    if ((s->unk43E & 3) == 0) {
        if (s->unk43E & 0x8000) {
            if (s->unk434 != 0) {
                func_ov000_022f8c28(data_022c4560, 0);
            } else {
                func_ov000_022f8bc8(data_022c4560, 0);
            }
        } else {
            if (s->unk434 != 0) {
                func_ov000_022f8c28(data_022c4560, 1);
            } else {
                func_ov000_022f8bc8(data_022c4560, 1);
            }
        }
        s->unk43E &= ~3;
        s->unk43E ^= 0x8000;
    }
    if (s->unk43C != 0) {
        s->unk43C--;
    }
    if (s->unk43C != 0) {
        return;
    }
    if (s->unk43E & 0x8000) {
        return;
    }
    func_ov000_022f4cbc(s);
    s->unk68 = func_ov000_022f369c;
}
