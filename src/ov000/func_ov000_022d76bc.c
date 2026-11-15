#include <nitro/types.h>

typedef struct UnkStruct_022d76bc_2 {
    u8 pad[0x17c];
    s32 unk17C;
    u8 pad1[4];
    s32 unk184;
} UnkStruct_022d76bc_2;

typedef struct UnkStruct_022d76bc {
    u8 pad[0x68];
    void (*func)(void);
    u8 pad1[0x258 - 0x6c];
    u16 unk258;
} UnkStruct_022d76bc;

extern UnkStruct_022d76bc_2 *data_ov000_0236ac60;
extern u32 data_0208fe70;

u16 func_020027a4(s32 a, s32 b);
void func_ov000_022d7630(void);

static inline u16 Rand(void) {
    data_0208fe70 = data_0208fe70 * 0x196225 + 0x3c6ef35f;
    return (u16)(data_0208fe70 >> 16);
}

void func_ov000_022d76bc(UnkStruct_022d76bc *s) {
    s->unk258 = func_020027a4(data_ov000_0236ac60->unk17C, data_ov000_0236ac60->unk184);
    s->unk258 += (Rand() & 0xf) * 0x2aa - 0x1555;
    s->func = func_ov000_022d7630;
}
